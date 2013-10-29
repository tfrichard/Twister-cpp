//
//  reduce_task.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/tasks/reduce_task.h>
#include <Twister-cpp/message/reduce_input_request_msg.h>
#include <boost/asio.hpp>

namespace twister {
    void reduce_task::run_reduce_function() {
        if (collector == NULL) {
            collector = new reduce_output_collector(job_conf.iteration, reducer_conf.reduce_task_no);
        }
        
        std::map<const key*, std::vector<const value*>> &reduce_inputs = reduce_input_.key_values;
        std::map<const key*, std::vector<const value*>>::iterator input_it = reduce_inputs.begin();
        for (; input_it != reduce_inputs.end(); input_it++) {
            reduce_instance->reduce(collector, input_it->first, input_it->second);
        }
    }
    
    void reduce_task::foo(std::vector<std::string> &daemons) {
        
        std::cout << "foo in reduce task" << std::endl;
    }
    
    void reduce_task::get_reduce_input(std::vector<std::string> &daemons) {
        reduce_input_request_msg reduce_input_request(this->reducer_conf.reduce_task_no);
        
        for (std::string ip_port : daemons) {
            std::size_t pos = ip_port.find(":");
            if (pos == std::string::npos) {
                continue;
            }
            std::string ip = ip_port.substr(0, pos);
            std::string port = ip_port.substr(pos + 1);
            
           // if (ip == this->daemon_ip) {
                
           // } else {
                //go remote host for reduce input
                try {
                    boost::asio::io_service io_service;
                    using boost::asio::ip::tcp;
                    
                    tcp::resolver resolver(io_service);
                    tcp::resolver::query query(ip, port);
                    tcp::resolver::iterator endpointer = resolver.resolve(query);
                    tcp::socket socket(io_service);
                    boost::asio::connect(socket, endpointer);
                    
                    boost::system::error_code ec;
                    
                    std::cout << "sending reduce input request to daemon: " << ip << std::endl;
                    out_archive out_ar;
                    out_ar << reduce_input_request;
                    
                    std::cout << "msg type is " << (MSG_TYPE)out_ar.buf[0] << std::endl;
                    boost::asio::write(socket, boost::asio::buffer(out_ar.buf, out_ar.len), ec);
                    
                    if (ec) {
                        std::cerr << "send msg failed in reduce input request" << std::endl;
                        exit(-1);
                    }
                    
                    //max lenghth for reduce input from one daemon
                    char *buf = new char[1024 * 1024];
                    memset(buf, 0, 1024 * 1024);
                    
                    std::size_t len = boost::asio::read(socket, boost::asio::buffer(buf, sizeof(int)), ec);
                    //std::size_t len = socket.read_some(boost::asio::buffer(buf, 1024*1024), ec);
                    if (len <= 0) {
                        std::cerr << "there is no data sent from the daemon " << ip << std::endl;
                        std::cerr << ec << std::endl;
                        continue;
                    }
                    
                    in_archive in_arc_len(buf, len);
                    int reduce_input_size = 0;
                    in_arc_len >> (int&)reduce_input_size;
                    
                    std::cout << "prepared to receive " << reduce_input_size << " bytes of data" << std::endl;
                    
                    memset(buf, 0, 1024 * 1024);
                    len = boost::asio::read(socket, boost::asio::buffer(buf, reduce_input_size), ec);
                    assert(len == reduce_input_size);
                    
                    std::cout << "received " << reduce_input_size << " bytes of data" << std::endl;
                    in_archive in_arc_data(buf, len);
                    
                    reduce_input input(in_arc_data);
                    reduce_input_ = input;
                    
                    delete buf;
                    
                } catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
    //}
}
