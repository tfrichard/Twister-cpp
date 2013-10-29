//
//  reduce_request_grp_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_request_grp_msg__
#define __Twister_cpp__reduce_request_grp_msg__

#include <iostream>
#include <Twister-cpp/message/reduce_request_msg.h>

namespace twister {
    class reduce_request_grp_msg : public message {
    public:
        int num_reduce_start_msg;
        std::vector<reduce_task_request_msg> reduce_start_requests;
        
        reduce_request_grp_msg() : num_reduce_start_msg(0) {
            msg_type = MSG_TYPE::START_REDUCE_TASK_GRP;
        }
        
        reduce_request_grp_msg(in_archive& in_arc) {
            load(in_arc);
        }
        
        ~reduce_request_grp_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> (int&)num_reduce_start_msg;
            for (int i = 0; i < num_reduce_start_msg; i++) {
                reduce_start_requests.emplace(reduce_start_requests.end(), reduce_task_request_msg(in_arc));
            }
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << num_reduce_start_msg;
            for (const reduce_task_request_msg& msg: reduce_start_requests) {
                out_arc << msg;
            }
        }
        
        void add_reduce_task_msg(const reduce_task_request_msg& msg) {
            reduce_start_requests.push_back(msg);
            num_reduce_start_msg += 1;
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_request_grp_msg__) */
