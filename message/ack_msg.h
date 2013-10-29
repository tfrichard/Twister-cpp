//
//  ack_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/7/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__ack_msg__
#define __Twister_cpp__ack_msg__

#include <iostream>
#include <Twister-cpp/message/message.h>

namespace twister {
    class ack_msg : public message {
    public:
        ACK_TYPE ack_type;
        
    public:
        ack_msg() {}
        ack_msg(ACK_TYPE ack_type_) : ack_type(ack_type_) { msg_type = MSG_TYPE::ACK; }
        ~ack_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type >> (int&)ack_type;
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type << ack_type;
        }

    };
}

#endif /* defined(__Twister_cpp__ack_msg__) */
