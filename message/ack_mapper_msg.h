//
//  ack_mapper_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/17/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__ack_mapper_msg__
#define __Twister_cpp__ack_mapper_msg__

#include <iostream>
#include <vector>
#include <algorithm>
#include <Twister-cpp/message/ack_msg.h>

namespace twister {
    class ack_mapper_msg : public ack_msg {
    public:
        int daemon_no;
        std::vector<int> reduce_inputs;
        
        ack_mapper_msg() {}
        ack_mapper_msg(ACK_TYPE ack_type_, int daemon_no_) : daemon_no(daemon_no_) {
            ack_type = ack_type_;
            msg_type = MSG_TYPE::ACK;
        }
        ~ack_mapper_msg() {}
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> (int&)ack_type;
            in_arc >> (int&)daemon_no;
            int num_inputs;
            in_arc >> (int&)num_inputs;
            for (int i = 0; i < num_inputs; i++) {
                int tmp;
                in_arc >> (int&)tmp;
                reduce_inputs.push_back(tmp);
            }
        }
        
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << ack_type;
            out_arc << daemon_no;
            out_arc << (int)reduce_inputs.size();
            for (int tmp : reduce_inputs) {
                out_arc << tmp;
            }
        }
        
        void add_reduce_input(int reduce_no) {
            if (find(reduce_inputs.begin(), reduce_inputs.end(), reduce_no) == reduce_inputs.end())
                reduce_inputs.push_back(reduce_no);
        }
    };
}

#endif /* defined(__Twister_cpp__ack_mapper_msg__) */
