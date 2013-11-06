//
//  daemon.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__daemon__
#define __Twister_cpp__daemon__

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <boost/asio.hpp>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/message/map_request_msg.h>
#include <Twister-cpp/message/reduce_request_msg.h>
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/tasks/map_task.h>
#include <Twister-cpp/tasks/reduce_task.h>

namespace twister {
    using boost::asio::ip::tcp;
    
	static std::atomic_flag mutex = ATOMIC_FLAG_INIT;

    class daemon {
    private:
        std::string ip_addr;
        std::string driver_ip;
        short port;
        int daemon_id;
        job_config* job_conf;
        
        std::map<int, map_task*> map_task_table;
        std::map<int, reduce_task*> reduce_task_table;
        
        tcp::acceptor acceptor;
        tcp::socket socket;
        
     
    public:
        daemon(boost::asio::io_service&, const tcp::endpoint &, int, const std::string&, const std::string&);
        virtual ~daemon();
        void foo();
        
    private:
        void do_accept();
        void process_msg(tcp::socket&&);
        void start_map_tasks(map_task_request_msg*);
        void start_reduce_tasks(reduce_task_request_msg*);
        
    };
}

#endif /* defined(__Twister_cpp__daemon__) */
