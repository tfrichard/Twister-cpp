//
//  map_start_grp_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/17/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_start_grp_msg__
#define __Twister_cpp__map_start_grp_msg__

#include <iostream>
#include <vector>
#include <Twister-cpp/message/map_start_msg.h>

namespace  twister {
    class map_start_grp_msg : public message {
    public:
        int num_map_start_msg;
        std::vector<map_start_msg> map_start_requests;
        
        map_start_grp_msg() : num_map_start_msg(0) {
            msg_type = MSG_TYPE::START_MAPPER_GRP;
        }
        
        map_start_grp_msg(in_archive& in_arc) {
            load(in_arc);
        }
        
        ~map_start_grp_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> (int&)num_map_start_msg;
            for (int i = 0; i < num_map_start_msg; i++) {
                map_start_requests.emplace(map_start_requests.end(), map_start_msg(in_arc));
            }
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << num_map_start_msg;
            for (const map_start_msg& msg: map_start_requests) {
                out_arc << msg;
            }
        }
        
        void add_map_task_msg(const map_start_msg& msg) {
            map_start_requests.push_back(msg);
            num_map_start_msg += 1;
        }
  
    };
}

#endif /* defined(__Twister_cpp__map_start_grp_msg__) */
