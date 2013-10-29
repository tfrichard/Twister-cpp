//
//  reduce_start_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_start_msg__
#define __Twister_cpp__reduce_start_msg__

#include <iostream>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

namespace twister {
    class reduce_start_msg : public message {
    public:
        reduce_start_msg() {
            msg_type = MSG_TYPE::START_REDUCER;
        }
        
        ~reduce_start_msg() {}
        
        void load(in_archive& in_ar) {
            in_ar >> (int&)msg_type;
        }
        
        void save(out_archive& out_ar) const {
            out_ar << msg_type;
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_start_msg__) */
