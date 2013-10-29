//
//  map_request.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/26/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_request__
#define __Twister_cpp__map_request__

#include <iostream>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/config/mapper_config.h>

namespace twister {
    class map_task_request_msg : public message {
        
    public:
        map_task_request_msg() {
            msg_type = MSG_TYPE::START_MAP_TASK;
        }
        
        map_task_request_msg(const job_config &job_conf_, const mapper_config &map_conf_) :
        job_conf(job_conf_), mapper_conf(map_conf_)
        {
            msg_type = MSG_TYPE::START_MAP_TASK;
        }
        
        map_task_request_msg(in_archive& i_arc) {
            load(i_arc);
        }
        
        ~map_task_request_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> job_conf;
            in_arc >> mapper_conf;
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << job_conf;
            out_arc << mapper_conf;
        }
        
    public:
        job_config job_conf;
        mapper_config mapper_conf;
    };
}

#endif /* defined(__Twister_cpp__map_request__) */
