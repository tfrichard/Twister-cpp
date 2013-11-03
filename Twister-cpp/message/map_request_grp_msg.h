//
//  map_request_grp_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/17/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_request_grp_msg__
#define __Twister_cpp__map_request_grp_msg__

#include <iostream>
#include <vector>
#include <Twister-cpp/message/map_request_msg.h>

namespace twister {
    class map_request_grp_msg : public message {
    public:
        int num_msgs;
        std::vector<map_task_request_msg> map_task_requests;
        
        map_request_grp_msg() : num_msgs(0) {
            msg_type = MSG_TYPE::START_MAP_TASK_GRP;
        }
        
        map_request_grp_msg(in_archive& in_arc) {
            load(in_arc);
        }
        
        ~map_request_grp_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> (int&)num_msgs;
            for (int i = 0; i < num_msgs; i++) {
                map_task_requests.emplace(map_task_requests.end(), map_task_request_msg(in_arc));
            }
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << num_msgs;
            for (const map_task_request_msg& msg: map_task_requests) {
                out_arc << msg;
            }
        }
        
        void add_map_task_msg(const map_task_request_msg& msg) {
            map_task_requests.push_back(msg);
            num_msgs += 1;
        }
    };
}

#endif /* defined(__Twister_cpp__map_request_grp_msg__) */
