//
//  reduce_input_request_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_input_request_msg__
#define __Twister_cpp__reduce_input_request_msg__

#include <iostream>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

namespace twister {
    class reduce_input_request_msg :  public message {
    public:
        int task_no;
        
        reduce_input_request_msg() {
            msg_type = MSG_TYPE::REDUCE_INPUT_REQUEST;
            task_no = -1;
        }
        
        reduce_input_request_msg(int task_no_) {
            task_no = task_no_;
            msg_type = MSG_TYPE::REDUCE_INPUT_REQUEST;
        }
        
        reduce_input_request_msg(in_archive& in_ar) {
            load(in_ar);
        }
        
        ~reduce_input_request_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> (int&)task_no;
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << task_no;
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_input_request_msg__) */
