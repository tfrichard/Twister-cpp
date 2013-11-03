//
//  map_task.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_task__
#define __Twister_cpp__map_task__

#include <iostream>
#include <map>
#include <Twister-cpp/worker/mapper.h>
#include <Twister-cpp/config/mapper_config.h>
#include <Twister-cpp/message/map_start_msg.h>
#include <Twister-cpp/worker/map_output_collector.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>

namespace twister {
    class map_task {
    public:
        int daemon_id;
        mapper* map_instance;
        map_output_collector* collector;
        job_config job_conf;
        mapper_config mapper_conf;
        //std::map<key, value> key_values;
        
        map_task() : daemon_id(0), map_instance(NULL), collector(NULL) {
        }
        
        map_task(int daemon_id_, const job_config& job_conf_, const mapper_config& mapper_conf_) : daemon_id(daemon_id_), map_instance(NULL), collector(NULL)
        {
            job_conf = job_conf_;
            mapper_conf = mapper_conf_;
        }
        
        void run_map_function(const map_start_msg&);
        
        ~map_task() {
            delete map_instance;
        }
    };
}
#endif /* defined(__Twister_cpp__map_task__) */
