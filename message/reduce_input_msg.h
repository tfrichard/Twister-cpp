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
        int task_no;
        std::vector<std::string> reduce_input_daemon_ips;
        
        reduce_input_msg() {
            msg_type = MSG_TYPE::REDUCE_INPUT_MSG;
            task_no = -1;
        }
        
        reduce_input_msg(int task_no_, std::vector<std::string> &reduce_input_daemons) {
            msg_type = MSG_TYPE::REDUCE_INPUT_MSG;
            task_no = task_no_;
            reduce_input_daemon_ips = reduce_input_daemons;
        }
        
        reduce_input_msg(in_archive& in_ar) {
            load(in_ar);
        }
        
        ~reduce_input_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> (int&)task_no;
            int num_of_daemons = 0;
            in_arc >> (int&)num_of_daemons;
            for (int i = 0; i < num_of_daemons; i++) {
                std::string tmp = "";
                in_arc >> tmp;
                reduce_input_daemon_ips.push_back(tmp);
            }
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << task_no;
            int num_of_daemons = reduce_input_daemon_ips.size();
            out_arc << num_of_daemons;
            for (std::vector<std::string>::const_iterator it = reduce_input_daemon_ips.begin();
                 it != reduce_input_daemon_ips.end(); it++) {
                out_arc << *it;
            }
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_input_msg__) */
