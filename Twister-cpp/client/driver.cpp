//
//  driver.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/client/driver.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/string_key.h>
#include <Twister-cpp/types/string_value.h>
#include <Twister-cpp/communication/conn_manager.h>
#include <Twister-cpp/client/synchronizer.h>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/message/map_request_msg.h>
#include <Twister-cpp/message/map_start_msg.h>
#include <Twister-cpp/message/map_request_grp_msg.h>
#include <Twister-cpp/message/map_start_grp_msg.h>
#include <Twister-cpp/message/reduce_request_grp_msg.h>
#include <Twister-cpp/message/reduce_request_msg.h>
#include <Twister-cpp/message/reduce_input_msg.h>
#include <Twister-cpp/message/gather_start_msg.h>
#include <Twister-cpp/serialization/out_archive.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/utility/class_factory.h>

#include <boost/foreach.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <utility>
#include <thread>

namespace twister{
    //log4cxx::LoggerPtr driver::log = log::get_logger("driver");
    
    driver::driver() {
        this->jobconf = job_config::getInstance();
        this->jobstate = job_state::getInstance();
        this->sync_mgr = new synchronizer(this, jobconf, jobstate);
		this->gather_ = NULL;
        
        //test connect with all daemons
        std::pair<unsigned int, std::string> item;
        std::vector<std::string> daemon_ips;
        BOOST_FOREACH(item, jobconf->daemon_map) {
            daemon_ips.push_back(item.second);
        }
        
        BOOST_FOREACH(std::string ip, daemon_ips) std::cout << ip << std::endl;
        //sleep(100);
        
#ifdef _DEBUG
        std::cout << "about to start all daemons by default env" << std::endl;
        conn_mgr = new conn_manager();
        conn_mgr->spawn_daemons_from_env();
        std::cout << "about to test connection" << std::endl;
        test_conn(daemon_ips);
        sleep(10);
#endif
        
    }
    
    driver::driver(job_config *job_conf) {
        jobconf = job_conf;
        jobstate = job_state::getInstance();
        sync_mgr = new synchronizer(this, jobconf, jobstate);
		gather_ = NULL;

        std::pair<unsigned int, std::string> item;
        std::vector<std::string> daemon_ips;
        BOOST_FOREACH(item, jobconf->daemon_map) {
            daemon_ips.push_back(item.second);
        }
        
        BOOST_FOREACH(std::string ip, daemon_ips) std::cout << ip << std::endl;
        
        conn_mgr = new conn_manager();
        conn_mgr->spawn_daemons_from_env();
        
        //configDaemons();
        config_map_tasks();
    }
    
    driver::~driver() {}
    
    void driver::config_map_tasks() {
        int num_of_mappers = jobconf->num_mappers;
        
        map_request_grp_msg map_task_grp;
        for (int map_task_no = 0; map_task_no < num_of_mappers; map_task_no++) {
            mapper_config mapper_conf(map_task_no, jobconf->partition_path[map_task_no], jobconf->udf_bin_path);
            map_task_request_msg start_map_task_msg(*jobconf, mapper_conf);
            map_task_grp.add_map_task_msg(start_map_task_msg);
        }
        
        if (!conn_mgr->bcast_msg(map_task_grp)) {
            std::cerr << "failed to bcast map task request" << std::endl;
            exit(-1);
        }

        jobstate->set_job_running_state(job_exec_state::MAP_TASK_STARTED);
        std::cout << "map tasks started successfully on daemons" << std::endl;
        
    }
    
    void driver::config_reduce_tasks_bcast() {
        //num_of_reducers here means num of reducers per daemon
        int num_of_reducers = jobconf->num_reducers;
        
        reduce_request_grp_msg reduce_task_grp;
        for (int reduce_task_no = 0; reduce_task_no < num_of_reducers; reduce_task_no++) {
            reducer_config reducer_conf(reduce_task_no, jobconf->reduce_path);
            //-1 means we don't really care daemon id here
            reduce_task_request_msg start_reduce_task_msg(-1, *jobconf, reducer_conf);
            reduce_task_grp.add_reduce_task_msg(start_reduce_task_msg);
        }
        
        if (!conn_mgr->bcast_msg(reduce_task_grp)) {
            std::cerr << "failed to bcast reduce task request" << std::endl;
            exit(-1);
        }
        
        jobstate->set_job_running_state(job_exec_state::REDUCE_TASK_STARTED);
        std::cout << "reduce tasks started successfully on daemons" << std::endl;
    }
    
    void driver::config_reduce_tasks() {
        int num_of_reducers = jobconf->num_reducers;
        
        for (int task_no = 0; task_no < num_of_reducers; task_no++) {
            reducer_config reducer_conf(task_no, jobconf->reduce_path);
            int deamon_id = task_no % conn_mgr->num_of_managed_daemon;
            reduce_task_request_msg start_reduce_task_msg(deamon_id, *jobconf, reducer_conf);
            if (!conn_mgr->send_msg(start_reduce_task_msg, deamon_id)) {
                std::cerr << "failed to start reduce task " << task_no << " on daemon " << deamon_id << std::endl;
                exit(-1);
            }
        }
        
        jobstate->set_job_running_state(job_exec_state::REDUCE_TASK_STARTED);
        std::cout <<"reduce tasks started successfully on daemons" << std::endl;
    }
    
    void driver::config_reduce_inputs() {
        std::map<int, std::vector<int>> &reduce_input_map = conn_mgr->reduce_input_deamon_map;
        int num_of_daemons = conn_mgr->num_of_managed_daemon;
   		
		for (int task_no = 0; task_no < jobconf->num_reducers; task_no++) {
			for (int daemon_no = 0; daemon_no < num_of_daemons; daemon_no++) {
				if (reduce_input_map.find(daemon_no) == reduce_input_map.end()) {
					std::cout << "there's no key hashed to daemon " << daemon_no << std::endl;
					continue;
				}
				
				std::string daemon_ip_port = conn_mgr->daemon_map[daemon_no] + ":" + std::to_string(12500 + daemon_no);
				reduce_input_msg reduce_input(daemon_ip_port, reduce_input_map[daemon_no]);
				//the send_msg should be async here, otherwise all daemons cannot request reduce input in parallel
				if (!conn_mgr->send_msg(reduce_input, task_no % conn_mgr->num_of_managed_daemon)) {
					std::cerr << "failed to send reduce input message  to daemon " << daemon_no << std::endl;
					exit(-1);
				}
			}
		}
        
    }
    
    void driver::test_conn(std::vector<std::string> ips) {
        try {
            boost::asio::io_service io_service;
            using boost::asio::ip::tcp;
            
            tcp::resolver resolver(io_service);
            
            BOOST_FOREACH(std::string ip, ips) {
                //std::cout << ip << std::endl;
                tcp::resolver::query query(ip, "12500");
                tcp::resolver::iterator endpointer = resolver.resolve(query);
                tcp::socket socket(io_service);
                boost::asio::connect(socket, endpointer);
                //boost::array<char, 128> buf;
                char buf[1024];
                memset(buf, 0, 1024);
                boost::system::error_code error;
                
                std::string test_str = "test connection";
                out_archive out_arc;
                out_arc << test_str;
                
                boost::system::error_code ignored_error;
                //boost::asio::write(socket, boost::asio::buffer(start_map_msg.data(), msg_size), ignored_error);
                std::cout << "out archive buf len is " << out_arc.len << std::endl;
                boost::asio::write(socket, boost::asio::buffer(out_arc.buf, out_arc.len), ignored_error);
                
                std::cout << "try to get response from server" << std::endl;
                size_t len = socket.read_some(boost::asio::buffer(buf), error);
                
                if (error == boost::asio::error::eof) exit(1);
                std::cout << std::string(buf) << "transferred " << len << " size of message to server successfully" << std::endl;
                                
                //sleep(100);
            }

        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    
    bool driver::is_mapreduce_completed() {
        return jobstate->job_running_state == job_exec_state::JOB_COMPLETED;
    }
    
    void driver::start_map(value* val) {
        jobstate->set_job_running_state(job_exec_state::MAPPER_STARTED);
        
        map_start_grp_msg map_start_request;
        for (int map_task_no = 0; map_task_no < jobconf->num_mappers; map_task_no++) {
            map_start_msg map_start(jobconf->job_name, map_task_no, "string_key", val->get_class_name());
            map_start.add_key_values(new string_key(jobconf->job_name + std::to_string(map_task_no)), val);
            for (std::map<const key*,const value*>::iterator it = map_start.key_value_table.begin(); it != map_start.key_value_table.end(); it++) {
                std::cout << static_cast<const string_key*>(it->first)->key_str << std::endl;
            }
            map_start_request.add_map_task_msg(map_start);
        }
        
        if (!conn_mgr->bcast_msg(map_start_request)) {
            std::cerr << "failed to bcast map start request" << std::endl;
            exit(-1);
        }
        
        jobstate->set_job_running_state(job_exec_state::MAPPER_COMPLETED);
        std::cout << "mapper function completed successfully on daemons" << std::endl;
        
    }
    
    void driver::start_shuffle() {
        jobstate->set_job_running_state(job_exec_state::SHUFFLE_STARTED);
        std::cout << "data shuffle starting from here" << std::endl;
        config_reduce_tasks();
        config_reduce_inputs();
        jobstate->set_job_running_state(job_exec_state::SHUFFLE_COMPLETED);
    }
    
    void driver::start_reduce() {
        jobstate->set_job_running_state(job_exec_state::REDUCER_STARTED);
        std::cout << "reducer works starting from here" << std::endl;
        reduce_start_msg reduce_start;
        if (!conn_mgr->bcast_msg(reduce_start)) {
            std::cerr << "failed to bcast reduce start request" << std::endl;
            exit(-1);
        }
        
        jobstate->set_job_running_state(job_exec_state::REDUCER_COMPLETED);
    }
    
    void driver::start_gather() {
        jobstate->set_job_running_state(job_exec_state::GATHER_STARTED);
        std::cout << "gather works starting from here" << std::endl;
        //start a tcp server to receive gather_input
        std::thread on_event_thread(&driver::on_event, this);
        on_event_thread.detach();
        sleep(1);
        
        gather_start_msg gather_start;
        if (!conn_mgr->bcast_msg(gather_start)) {
            std::cerr << "failed to bcast gather start request" << std::endl;
            exit(-1);
        }
        
        jobstate->set_job_running_state(job_exec_state::GATHER_COMPLETED);
        jobstate->set_job_running_state(job_exec_state::JOB_COMPLETED);
    }
    
    void driver::on_event() {
        typedef boost::shared_ptr<tcp::socket> socket_ptr;
        
        std::cout << "driver listen up..." << std::endl;
        
        try {
            boost::asio::io_service io_service;
            io_service.run();
            tcp::endpoint end_point(tcp::v4(), 10000);
            tcp::acceptor acceptor(io_service, end_point);
            
            for (; ; ) {
                
                socket_ptr sock(new  tcp::socket(io_service));
                
                std::cout << "ready to accept daemon request" << std::endl;
                acceptor.accept(*sock);
                std::cout << "connection accepted" << std::endl;
                
                char* received_data = new char[1024];
                boost::system::error_code ec;
                //std::size_t len = socket.read_some(boost::asio::buffer(received_data), ec);
                std::size_t len = boost::asio::read(*sock, boost::asio::buffer(received_data, sizeof(int)), ec);
                if (ec) {
                    std::cerr << "driver msg receive error!" << std::endl;
                    exit(-1);
                }
                assert(len == sizeof(int));
                in_archive in_ar_len(received_data, len);
                int msg_size = 0;
                in_ar_len >> msg_size;
                delete received_data;
                
                received_data = new char[msg_size + 1];
                memset(received_data, 0,  msg_size + 1);
                len = boost::asio::read(*sock, boost::asio::buffer(received_data, msg_size), ec);
                if (ec) {
                    std::cerr << "driver msg receive error!" << std::endl;
                    exit(-1);
                }
                assert(len == msg_size);
                
                std::cout << "driver received msg with size: " << len << std::endl;
                
                MSG_TYPE msg_type = (MSG_TYPE)received_data[0];
                std::cout << "msg type is: " << msg_type << std::endl;
                
                switch (msg_type) {
                    case GATHER_INPUT:
                    {
                        receive_gather_input(received_data, msg_size);
                        ack_msg gather_input_ack(ACK_TYPE::GATHER_INPUT_ACK);
                        out_archive out_ar;
                        out_ar << gather_input_ack;
                        boost::asio::write(*sock, boost::asio::buffer(out_ar.buf, out_ar.len), ec);
                        
                    }
                        break;
                        
                    default:
                        break;
                }
                delete received_data;
                sock->close();
            }
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    
    void driver::receive_gather_input(char* buf, int len) {
        if (gather_ == NULL) {
            class_factory::add_class_path(jobconf->udf_bin_path);
            class_factory::registe_class(jobconf->gather_name, jobconf->udf_dy_lib);
            gather_t_ptr* gather_hook = (gather_t_ptr*)class_factory::get_instance(jobconf->gather_name);
            gather_ = gather_hook();
        }
        
        in_archive in_ar(buf, len);
        gather_input gather_input_(in_ar);
        
        gather_->do_gather(gather_input_.key_value_output);
    }
    
    bool driver::runMapReduceJob(std::vector<std::pair<key, value> > input_pairs) {
        
        return true;
    };
    
    bool driver::runMapReduceJob(value* val) {
        //should be $TWISTER_HOME/bin in release
        class_factory::add_class_path("/ccs/home/feiteng/twistercpp/lib/");
        
        start_map(val);
        std::thread sync_thread(&synchronizer::main_sync_loop, sync_mgr);
        sync_thread.join();
        class_factory::destory();
        return true;
    }
    
}
