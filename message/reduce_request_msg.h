//
//  reduce_request_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_request_msg__
#define __Twister_cpp__reduce_request_msg__

#include <iostream>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/config/reduce_config.h>
#include <Twister-cpp/config/job_config.h>

namespace twister {
        class reduce_task_request_msg : public message {
            
        public:
            reduce_task_request_msg() {
                msg_type = MSG_TYPE::START_REDUCE_TASK;
                daemon_id = -1;
            }
            
            reduce_task_request_msg(int deamon_id_, const job_config &job_conf_, const reducer_config &reduce_conf_) :
            job_conf(job_conf_), reducer_conf(reduce_conf_)
            {
                msg_type = MSG_TYPE::START_REDUCE_TASK;
                daemon_id = deamon_id_;
            }
            
            reduce_task_request_msg(in_archive& i_arc) {
                load(i_arc);
            }
            
            ~reduce_task_request_msg() {}
            
            void load(in_archive& in_arc) {
                in_arc >> (int&)msg_type;
                in_arc >> daemon_id;
                in_arc >> job_conf;
                in_arc >> reducer_conf;
            }
            
            void save(out_archive& out_arc) const {
                out_arc << msg_type;
                out_arc << daemon_id;
                out_arc << job_conf;
                out_arc << reducer_conf;
            }
            
        public:
            int daemon_id;
            job_config job_conf;
            reducer_config reducer_conf;
        };
}

#endif /* defined(__Twister_cpp__reduce_request_msg__) */
