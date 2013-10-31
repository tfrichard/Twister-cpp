//
//  daemon.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/worker/daemon.h>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/message/map_request_msg.h>
#include <Twister-cpp/message/map_start_msg.h>
#include <Twister-cpp/message/map_request_grp_msg.h>
#include <Twister-cpp/message/map_start_grp_msg.h>
#include <Twister-cpp/message/reduce_request_msg.h>
#include <Twister-cpp/message/reduce_request_grp_msg.h>
#include <Twister-cpp/message/ack_msg.h>
#include <Twister-cpp/message/ack_mapper_msg.h>
#include <Twister-cpp/message/reduce_input_msg.h>
#include <Twister-cpp/message/reduce_input_request_msg.h>
#include <Twister-cpp/worker/mapper.h>
#include <Twister-cpp/worker/reducer.h>
#include <Twister-cpp/tasks/map_task.h>
#include <Twister-cpp/tasks/reduce_task.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/string_key.h>
#include <Twister-cpp/types/string_value.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <dlfcn.h>
#include <unistd.h>

namespace twister {
    using boost::asio::ip::tcp;
    
    
    daemon::daemon(boost::asio::io_service &io_service_, const tcp::endpoint &end_point_) : \
    acceptor(io_service_, end_point_), socket(io_service_)
    {
        //should be $TWISTER_HOME/bin in release
        class_factory::add_class_path("/Users/feiteng/Library/Developer/Xcode/DerivedData/Twister-cpp-bhxrgrfnxsvvhwfeqbijayxvhjpd/Build/Products/Debug/");
        
        daemon_id = 0;
        ip_addr = "127.0.0.1";
        driver_ip = "127.0.0.1";
        do_accept();
        port = end_point_.port();
    }
    
    daemon::~daemon(){}
    
    void daemon::do_accept() {
        //start up the daemon server
        std::string msg("I'm daemon server!");
        
        //char received_data[];
        
        try {
            for (;;) {
                tcp::socket socket_(std::move(socket));
                std::cout << "daemon is listening..." << std::endl;
                acceptor.accept(socket_);
                std::cout << "connection accepted" << std::endl;
                boost::system::error_code ignored_error;
                
                std::thread proc_thread(&daemon::process_msg, this, std::move(socket_));
                proc_thread.detach();
            }
            
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

    }
    
    void daemon::process_msg(tcp::socket &&socket) {
        char received_data[8192];
        boost::system::error_code ec;
        std::size_t len = socket.read_some(boost::asio::buffer(received_data), ec);
        
        if (ec) {
            std::cerr << "daemon msg receive error!" << std::endl;
            exit(-1);
        }
        
        std::cout << "daemon received msg with size: " << len << std::endl;
        
        in_archive in_arc(received_data, len);
        
        MSG_TYPE msg_type = (MSG_TYPE)received_data[0];
        std::cout << "msg type is: " << msg_type << std::endl;
        
        if (msg_type == START_MAP_TASK) {
            map_task_request_msg map_msg(in_arc);
           
            std::cout << "to start new " << map_msg.job_conf.num_mappers << " mappers on daemon: " << this->daemon_id << std::endl;
            
            std::thread map_task_thread(&daemon::start_map_tasks, this, &map_msg);
            std::cout << "thread " << map_task_thread.get_id() << " is started as map task instance" << std::endl;
            
            map_task_thread.join();
            std::cout << "thread joined" << std::endl;
            
            ack_msg ack_map_task(ACK_TYPE::MAP_TASK_STARTED_ACK);
            out_archive out_iac;
            out_iac << ack_map_task;
            //sleep(3);
            boost::asio::write(socket, boost::asio::buffer(out_iac.buf, out_iac.len), ec);
            
        } else if (msg_type == START_MAP_TASK_GRP) {
            map_request_grp_msg map_task_grp_msg(in_arc);
            int num_of_mappers = map_task_grp_msg.num_msgs;
            std::vector<map_task_request_msg>& request_copy = map_task_grp_msg.map_task_requests;
            std::cout << "to start new " << num_of_mappers << " mappers on daemon: " << this->daemon_id << std::endl;
            
            std::vector<std::thread> threads;
            for (int i = 0; i < num_of_mappers; i++) {
                threads.push_back(std::thread(&daemon::start_map_tasks, this, &request_copy[i]));
                std::cout << "thread " << threads[i].get_id() << " is started as map task instance" << std::endl;
            }
            
            for (std::thread& map_thread : threads) {
                map_thread.join();
            }
            std::cout << "[map_task_request]: all threads joined" << std::endl;
            
            ack_msg ack_map_task(ACK_TYPE::MAP_TASK_STARTED_GRP_ACK);
            out_archive out_iac;
            out_iac << ack_map_task;
            //sleep(3);
            boost::asio::write(socket, boost::asio::buffer(out_iac.buf, out_iac.len), ec);
        } else if (msg_type == START_MAPPER_GRP) {
            map_start_grp_msg mapper_start(in_arc);
            int num_of_mappers = mapper_start.num_map_start_msg;
            std::vector<map_start_msg>& request_copy = mapper_start.map_start_requests;
            
            std::vector<std::thread> threads;
            for (int i = 0; i < num_of_mappers; i++) {
                map_task* cur_map_task = map_task_table[request_copy[i].map_task_no];
                threads.push_back(std::thread(&map_task::run_map_function, cur_map_task, request_copy[i]));
            }
            
            for (std::thread& mapper_thread : threads) {
                mapper_thread.join();
            }
            std::cout << "[mapper_start]: all mapper finished execution" << std::endl;
            
            ack_mapper_msg ack_mapper(ACK_TYPE::MAP_START_GRP_ACK, daemon_id);
            for (std::map<int, map_task*>::iterator it = map_task_table.begin(); it != map_task_table.end(); it++) {
                map_output_collector *cur_collector = it->second->collector;
                for (std::map<int, int>::iterator iter = cur_collector->reduce_input_cnt_table.begin(); iter != cur_collector->reduce_input_cnt_table.end(); iter++) {
                    ack_mapper.add_reduce_input(iter->first);
                }
            }
            out_archive out_iac;
            out_iac << ack_mapper;
            std::size_t len = boost::asio::write(socket, boost::asio::buffer(out_iac.buf, out_iac.len), ec);
        } else if (msg_type == START_MAPPER) {
            map_start_msg map_start(in_arc);
            
            map_task* cur_map_task = map_task_table[map_start.map_task_no];
            std::thread map_thread(&map_task::run_map_function, cur_map_task, map_start);
            map_thread.join();
            
            std::cout << "mapper : " << map_start.map_task_no << " finished execution" << std::endl;
            
            ack_msg ack_mapper(ACK_TYPE::MAP_START_ACK);
            out_archive out_iac;
            out_iac << ack_mapper;
            boost::asio::write(socket, boost::asio::buffer(out_iac.buf, out_iac.len), ec);
            
        } else if (msg_type == START_REDUCE_TASK_GRP) {
            reduce_request_grp_msg reducer_start(in_arc);
            int num_of_reducers = reducer_start.num_reduce_start_msg;
            std::vector<reduce_task_request_msg>& request_copy = reducer_start.reduce_start_requests;
            
            std::cout << "to start new " << num_of_reducers << " reducers on daemon: " << this->daemon_id << std::endl;
            
            std::vector<std::thread> threads;
            for (int i = 0; i < num_of_reducers; i++) {
                threads.push_back(std::thread(&daemon::start_reduce_tasks, this, &request_copy[i]));
                std::cout << "thread " << threads[i].get_id() << " is started as reduce task instance" << std::endl;
            }
            
            for (std::thread& map_thread : threads) {
                map_thread.join();
            }
            std::cout << "[reduce_task_request]: all threads joined" << std::endl;
            
            ack_msg ack_reduce_task(ACK_TYPE::REDUCE_TASK_STARTED_GRP_ACK);
            out_archive out_iac;
            out_iac << ack_reduce_task;
            //sleep(3);
            boost::asio::write(socket, boost::asio::buffer(out_iac.buf, out_iac.len), ec);
        } else if (msg_type == START_REDUCE_TASK) {
            reduce_task_request_msg reducer_start(in_arc);
            std::cout << "to start a new reducer task on daemon " << this->ip_addr << std::endl;
            std::thread thread(&daemon::start_reduce_tasks, this, &reducer_start);
            std::cout << "thread " << thread.get_id() << " is started as reduce task instance" << std::endl;
            thread.join();
            std::cout << "[reduce_task_request]: thread is joined" << std::endl;
            
            ack_msg ack_reduce_task(ACK_TYPE::REDUCE_TASK_STARTED_ACK);
            out_archive out_ar;
            out_ar << ack_reduce_task;
            boost::asio::write(socket, boost::asio::buffer(out_ar.buf, out_ar.len), ec);
            
        } else if (msg_type == REDUCE_INPUT_MSG) {
            reduce_input_msg reduce_input(in_arc);
            std::vector<int> &reduce_tasks = reduce_input.task_nos;
            
            std::cout << "reduce task: " ;
            for (int i : reduce_tasks) {
                std::cout << i << " ";
            }
            std::cout << " are going to request reduce input in daemon " << this->daemon_id << std::endl;
            
            for (int task_no : reduce_tasks) {
                reduce_task* reduce_task_ptr = reduce_task_table[task_no];
                reduce_task_ptr->get_reduce_input(reduce_input.daemon_ip);
            }
            
            ack_msg ack_reduce_input(ACK_TYPE::REDUCE_INPUT_MSG_ACK);
            out_archive out_ar;
            out_ar << ack_reduce_input;
            boost::asio::write(socket, boost::asio::buffer(out_ar.buf, out_ar.len), ec);
            
        } else if (msg_type == REDUCE_INPUT_REQUEST) {
            reduce_input_request_msg reduce_input_request(in_arc);
            reduce_input reduce_input_data;
            int task_no = reduce_input_request.task_no;
            std::cout << "handle reduce input request from task no " << task_no << std::endl;
            
            for (std::map<int, map_task*>::iterator it = map_task_table.begin(); it != map_task_table.end(); it++) {
                map_task* map_task_ptr = it->second;
                map_output_collector* collector = map_task_ptr->collector;
                //merge all key value tuples into reduce_input_data and send it
                if (collector->reduce_input_table.find(task_no) != collector->reduce_input_table.end()) {
                    reduce_input& tmp = collector->reduce_input_table[task_no];
                    if (reduce_input_data.key_class == "" || reduce_input_data.value_class == "") {
                        reduce_input_data.key_class = tmp.key_class;
                        reduce_input_data.value_class = tmp.value_class;
                    }
                    std::map<const key*, std::vector<const value*>> &key_value_tuples = tmp.key_values;
                    std::map<const key*, std::vector<const value*>>::const_iterator it = key_value_tuples.begin();
                    for (; it != key_value_tuples.end(); it++) {
                        for (const value* val : it->second) {
                            reduce_input_data.add_key_value(it->first, val);
                            }
                        }
                    }
                }
            
            out_archive out_ar;
            out_ar << reduce_input_data;
            out_archive out_ar_len;
            out_ar_len << out_ar.len;
            boost::asio::write(socket, boost::asio::buffer(out_ar_len.buf, out_ar_len.off), ec);
            boost::asio::write(socket, boost::asio::buffer(out_ar.buf, out_ar.len), ec);
        
        } else if (msg_type == START_REDUCER) {
            std::cout << "all reducers starting now..." << std::endl;
            std::vector<std::thread> reduce_threads;
            std::map<int, reduce_task*>::iterator it = this->reduce_task_table.begin();
            for (; it != reduce_task_table.end(); it++) {
                reduce_task* reduce_task_ptr = it->second;
                reduce_threads.push_back(std::thread(&reduce_task::run_reduce_function, reduce_task_ptr));
            }
            
            for (std::thread &reduce_thread : reduce_threads) {
                reduce_thread.join();
            }
            std::cout << "[reduce start request] : all threads joined" << std::endl;
            
            ack_msg ack_reduce_start(ACK_TYPE::REDUCE_START_ACK);
            out_archive out_ar;
            out_ar << ack_reduce_start;
            
            boost::asio::write(socket, boost::asio::buffer(out_ar.buf, out_ar.len), ec);
            
        } else if (msg_type == START_GATHER) {
            if (!reduce_task_table.empty()) {
                std::cout << "gather works starting now..." << std::endl;
                try {
                    boost::asio::io_service io_service;
                    using boost::asio::ip::tcp;
                    
                    tcp::resolver resovler(io_service);
                    
                    std::string ip = driver_ip;
                    std::string port = std::to_string(10000);
                    tcp::resolver::query query(ip, port);
                    tcp::resolver::iterator endpointer = resovler.resolve(query);
                    
                    
                    boost::system::error_code error;
                    
                    std::cout << "try to send gather input to client driver: " << daemon_id << std::endl;
                    for (std::map<int, reduce_task*>::iterator it = reduce_task_table.begin(); it != reduce_task_table.end(); it++) {
                        if (it->second->collector->gather_input_.key_value_output.empty()) {
                            continue;
                        }
                        tcp::socket gather_socket(io_service);
                        boost::asio::connect(gather_socket, endpointer);
                        
                        out_archive out_ar;
                        out_ar << it->second->collector->gather_input_;
                        
                        std::cout << "msg type is " << (MSG_TYPE)out_ar.buf[0] << std::endl;
                        
                        std::cout << "gather input buf len is " << out_ar.len << std::endl;
                        out_archive out_ar_len;
                        out_ar_len << out_ar.len;
                        
                        boost::asio::write(gather_socket, boost::asio::buffer(out_ar_len.buf, out_ar_len.off), error);
                        if (error) {
                            std::cerr << "send gather input len failed in deamon " << daemon_id << std::endl;
                            std::cerr << error << std::endl;
                            gather_socket.close();
                            return;
                        }
                        
                        boost::asio::write(gather_socket, boost::asio::buffer(out_ar.buf, out_ar.len), error);
                        if (error) {
                            std::cerr << "send gather input data failed in deamon " << daemon_id << std::endl;
                            std::cerr << error << std::endl;
                            gather_socket.close();
                            return;
                        }
                        
                        char ack_buf[1024];
                        std::size_t len = gather_socket.read_some(boost::asio::buffer(ack_buf), error);
                        in_archive in_ar(ack_buf, len);
                        ack_msg gather_input_ack;
                        in_ar >> gather_input_ack;
                        if (gather_input_ack.ack_type != ACK_TYPE::GATHER_INPUT_ACK) {
                            std::cerr << "wrong ack type! expecting ACK_TYPE::GATHER_INPUT_ACK" << std::endl;
                            gather_socket.close();
                        } else {
                            std::cout << "task no: " << it->second->reducer_conf.reduce_task_no << " successfully send gather input to driver" << std::endl;
                        }
                         gather_socket.close();
                    }
                    
                } catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
                
                std::cout << "gather works finished now..." << std::endl;
                ack_msg gather_start_ack(ACK_TYPE::START_GATHER_ACK);
                out_archive ack_ar;
                ack_ar << gather_start_ack;
                
                boost::asio::write(socket, boost::asio::buffer(ack_ar.buf, ack_ar.len), ec);
            }
        } else {
            std::cerr << "not supported msg type" << std::endl;
            exit(-1);
        }
        
        socket.close();
    }
    
    void daemon::start_map_tasks(map_task_request_msg *map_request_msg) {
        map_task* map_task_ptr = new map_task(this->daemon_id, map_request_msg->job_conf, map_request_msg->mapper_conf);
        
        int map_task_no = map_request_msg->mapper_conf.map_task_no;
        
        while (mutex.test_and_set(std::memory_order_acquire)) {}
        map_task_table[map_task_no] = map_task_ptr;
        mutex.clear(std::memory_order_release);
        
        class_factory::add_class_path(map_task_ptr->job_conf.udf_bin_path);
        class_factory::registe_class(map_task_ptr->job_conf.mapper_name, map_task_ptr->job_conf.udf_dy_lib);
        mapper_t_ptr* mapper_hook = (mapper_t_ptr*)class_factory::get_instance(map_task_ptr->job_conf.mapper_name);
        
        map_task_ptr->map_instance = mapper_hook();
        job_config job_conf_tmp = map_task_ptr->job_conf;
        mapper_config mapper_conf_tmp = map_task_ptr->mapper_conf;
        
        map_task_ptr->map_instance->configure(&job_conf_tmp, &mapper_conf_tmp);

        //dlclose(dl_handle);
        
        std::cout << "map task starting thread about to exist" << std::endl;
    }
    
    void daemon::start_reduce_tasks(reduce_task_request_msg *reduce_request_msg) {
        reduce_task* reduce_task_ptr = new reduce_task(this->ip_addr, reduce_request_msg->job_conf, reduce_request_msg->reducer_conf);
        
        int reduce_task_no = reduce_request_msg->reducer_conf.reduce_task_no;
        
        while (mutex.test_and_set(std::memory_order_acquire)) {}
        reduce_task_table[reduce_task_no] = reduce_task_ptr;
        mutex.clear(std::memory_order_release);
        
        class_factory::add_class_path(reduce_task_ptr->job_conf.udf_bin_path);
        class_factory::registe_class(reduce_task_ptr->job_conf.reducer_name, reduce_task_ptr->job_conf.udf_dy_lib);
        reducer_t_ptr* reducer_hook = (reducer_t_ptr*)class_factory::get_instance(reduce_task_ptr->job_conf.reducer_name);
        
        reduce_task_ptr->reduce_instance = reducer_hook();
        job_config job_conf_tmp = reduce_task_ptr->job_conf;
        reducer_config reducer_conf_tmp = reduce_task_ptr->reducer_conf;
        
        reduce_task_ptr->reduce_instance->configure(&job_conf_tmp, &reducer_conf_tmp);
        
        //dlclose(dl_handle);
        
        std::cout << "reduce task starting thread about to exist" << std::endl;
    }

    
    void daemon::foo() {
        std::cout << "here's foo in daemon!" << std::endl;
    }
}
