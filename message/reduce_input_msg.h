//
//  reduce_input_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_input_msg__
#define __Twister_cpp__reduce_input_msg__

#include <iostream>
#include <vector>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

namespace twister {
    class reduce_input_msg : public message {
    public:
        std::string daemon_ip;
        std::vector<int> task_nos;
        
        reduce_input_msg() {
            msg_type = MSG_TYPE::REDUCE_INPUT_MSG;
            daemon_ip = "";
        }
        
        reduce_input_msg(std::string& daemon_ip_, std::vector<int> &task_nos_) {
            msg_type = MSG_TYPE::REDUCE_INPUT_MSG;
            daemon_ip = daemon_ip_;
            task_nos = task_nos_;
        }
        
        reduce_input_msg(in_archive& in_ar) {
            load(in_ar);
        }
        
        ~reduce_input_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> daemon_ip;
            int num_of_reducers = 0;
            in_arc >> (int&)num_of_reducers;
            for (int i = 0; i < num_of_reducers; i++) {
                int tmp = 0;
                in_arc >> (int&)tmp;
                task_nos.push_back(tmp);
            }
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << daemon_ip;
            int num_of_reducers = task_nos.size();
            out_arc << num_of_reducers;
            for (std::vector<int>::const_iterator it = task_nos.begin();
                 it != task_nos.end(); it++) {
                out_arc << *it;
            }
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_input_msg__) */
