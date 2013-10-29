//
//  reduce_task.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_task__
#define __Twister_cpp__reduce_task__

#include <iostream>
#include <set>
#include <Twister-cpp/config/reduce_config.h>
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/worker/reducer.h>
#include <Twister-cpp/worker/reduce_output_collector.h>
#include <Twister-cpp/message/reduce_request_msg.h>
#include <Twister-cpp/message/reduce_start_msg.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/reduce_input.h>
#include <boost/asio.hpp>

namespace twister {
    class reduce_task {
    public:
        std::string daemon_ip;
        reducer* reduce_instance;
        reduce_output_collector* collector;
        job_config job_conf;
        reducer_config reducer_conf;
        reduce_input reduce_input_;
        
        reduce_task() : daemon_ip(""), reduce_instance(NULL), collector(NULL) {
        }
        
        reduce_task(std::string &daemon_ip_, const job_config& job_conf_, const reducer_config& reducer_conf_) : daemon_ip(daemon_ip_), reduce_instance(NULL), collector(NULL)
        {
            job_conf = job_conf_;
            reducer_conf = reducer_conf_;
        }
        
        void run_reduce_function();
        
        void get_reduce_input(std::vector<std::string>&);
        
        void foo(std::vector<std::string>&);
        
        ~reduce_task() {
            delete reduce_instance;
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_task__) */
