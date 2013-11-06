//
//  conn_manager.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/communication/conn_manager.h>
#include <Twister-cpp/message/map_request_msg.h>
#include <Twister-cpp/message/ack_msg.h>
#include <Twister-cpp/message/ack_mapper_msg.h>
#include <boost/foreach.hpp>
#include <cstdlib>
#include <fstream>

namespace twister {
    conn_manager::conn_manager() {
        //ip_list.push_back("127.0.0.1");
        //ip_list.push_back("127.0.0.1");
        //daemon_map[0] = "127.0.0.1";
        //daemon_map[1] = "127.0.0.1";
        num_of_managed_daemon = 0;
		const char* twister_home = getenv("TWISTER_HOME");
		if (twister_home == NULL) {
			std::cerr << "please set TWISTER_HOME properly" << std::endl;
			exit(-1);
		}
		std::string node_file(twister_home);
		node_file += "/bin/nodes";

		std::ifstream node_list(node_file, std::ifstream::in);
		std::string ip = "";
		while (getline(node_list, ip)) {
			ip_list.push_back(ip);
			daemon_map[num_of_managed_daemon++] = ip;
		}
		node_list.close();
    }
    
    conn_manager::~conn_manager() {
    }
    
    bool conn_manager::spawn_daemons_from_env() {
        if (is_spawn_from_env()) {
            std::string command;
            char *twister_home = getenv("TWISTER_HOME");
            if (twister_home == NULL) {
                std::cerr << "please set TWISTER_HOME properly" << std::endl;
                return false;
            }
            char *hosts_file = getenv("HOST_FILE");
            if (hosts_file == NULL) {
                std::cerr << "HOST_FILE not set. Please startup daemon yourself" << std::endl;
                return false;
            }
            command = std::string(twister_home) + "/bin/spawn_daemon.py";
            std::system(command.c_str());
            return true;
        }
        else return false;
    }
    
    bool conn_manager::bcast_msg(const message &msg) {
        int num_of_suc_response = 0;
        
        try {
            boost::asio::io_service io_service;
            using boost::asio::ip::tcp;
            
            tcp::resolver resolver(io_service);
            std::vector<tcp::socket*> socket_vec;
            
			int daemon_id = 0;
            BOOST_FOREACH(std::string ip, ip_list) {
                //std::cout << ip << std::endl;
                tcp::resolver::query query(ip, std::to_string(12500 + daemon_id++));
                tcp::resolver::iterator endpointer = resolver.resolve(query);
                tcp::socket *socket = new tcp::socket(io_service);
                boost::asio::connect(*socket, endpointer);
                socket_vec.push_back(socket);
                
                boost::system::error_code error;
                
                std::cout << "try to send task request to daemon server" << std::endl;
                out_archive out_iac;
                out_iac << msg;
                
                std::cout << "msg type is: " << (MSG_TYPE)out_iac.buf[0] << std::endl;
                
                boost::system::error_code ignored_error;
                //boost::asio::write(socket, boost::asio::buffer(start_map_msg.data(), msg_size), ignored_error);
                std::cout << "out archive buf len is " << out_iac.len << std::endl;
                boost::asio::write(*socket, boost::asio::buffer(out_iac.buf, out_iac.len), ignored_error);
            }
            
            for (int i = 0; i < ip_list.size(); i++) {
                //buf used to receive daemon response
                char buf[1024];
                memset(buf, 0, 1024);
                
                boost::system::error_code error;
                std::cout << "try to get task starting response from daemon: " << ip_list[i] <<  std::endl;
                size_t len = socket_vec[i]->read_some(boost::asio::buffer(buf), error);
                //size_t len = boost::asio::read(*socket_vec[i], boost::asio::buffer(buf), error);
                if (error == boost::asio::error::eof) {
                    std::cout << "end of file in socket" << std::endl;
                    exit(1);
                }
                
                in_archive in_iac(buf, len);
                process_ack(in_iac, msg);
                
                std::cout << std::string(buf) << "transferred " << len << " size of message to daemon successfully" << std::endl;
                
                ++num_of_suc_response;
                socket_vec[i]->close();
                delete socket_vec[i];
            }
            
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
        
        return num_of_suc_response == ip_list.size();

    }
    
    bool conn_manager::send_msg(const message &msg, int daemon_id) {
        
        try {
            boost::asio::io_service io_service;
            using boost::asio::ip::tcp;
            
            tcp::resolver resovler(io_service);
            tcp::socket socket(io_service);
            std::string ip = daemon_map[daemon_id];
            std::string port = std::to_string(12500 + daemon_id);
            tcp::resolver::query query(ip, port);
            tcp::resolver::iterator endpointer = resovler.resolve(query);
            boost::asio::connect(socket, endpointer);
            
            boost::system::error_code error;
            
            std::cout << "try to send task request to daemon server: " << daemon_id << std::endl;
            out_archive out_arc;
            out_arc << msg;
            
            std::cout << "msg type is " << (MSG_TYPE)out_arc.buf[0] << std::endl;
            
            std::cout << "out archive buf len is " << out_arc.len << std::endl;
            boost::asio::write(socket, boost::asio::buffer(out_arc.buf, out_arc.len), error);
            
            if (error) {
                std::cerr << "send msg failed in conn_mgr: " << error << std::endl;
                return false;
            }
            
            char buf[1024];
            memset(buf, 0, 1024);
            
            std::cout << "try to get task starting response from daemon: " << daemon_id <<  std::endl;
            std::size_t len = socket.read_some(boost::asio::buffer(buf), error);
            if (error == boost::asio::error::eof) {
                std::cout << "end of file in socket" << std::endl;
                return false;
            }
            
            in_archive in_arc(buf, len);
            process_ack(in_arc, msg);
            
            std::cout << std::string(buf) << "transferred " << len << " size of message to daemon successfully" << std::endl;
            socket.close();
            
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
        
        return true;
    }
    
    void conn_manager::process_ack(in_archive& in_arc, const message &msg) {
        ack_msg daemon_ack;
        in_arc >> daemon_ack;
        
        if (daemon_ack.msg_type != MSG_TYPE::ACK) {
            std::cout << "wrong message type! Expecting ACK of map task completion" << std::endl;
            exit(1);
        }
        
        if (ack_table[daemon_ack.ack_type] != msg.msg_type) {
            std::cout << "wrong ack type! Expecting ack for MSG_TYPE::" << msg.msg_type << std::endl;
            exit(1);
        }

        in_arc.reset();
        switch (daemon_ack.ack_type) {
            case ACK_TYPE::MAP_START_GRP_ACK:
            {
                ack_mapper_msg mapper_ack;
                in_arc >> mapper_ack;
                reduce_input_deamon_map[mapper_ack.daemon_no] = mapper_ack.reduce_inputs;
                break;
            }
            default:
                break;
        }
    }
                
}
