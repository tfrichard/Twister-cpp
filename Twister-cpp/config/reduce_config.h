//
//  reduce_config.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_config__
#define __Twister_cpp__reduce_config__

#include <iostream>
#include <Twister-cpp/message/message.h>

namespace twister {
    class reducer_config : public message {
    public:
        int reduce_task_no;
        std::string reduce_path;
        
        reducer_config() {}
        
        reducer_config(unsigned int task_no, std::string reduce_path_) : reduce_task_no(task_no), reduce_path(reduce_path_) {}
        
        reducer_config(const reducer_config& other) {
            reduce_task_no = other.reduce_task_no;
            reduce_path = other.reduce_path;
        }
        
        reducer_config& operator=(const reducer_config& other) {
            reduce_task_no = other.reduce_task_no;
            reduce_path = other.reduce_path;
            
            return *this;
        }
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)reduce_task_no >> reduce_path;
        }
        
        void save(out_archive& out_arc) const {
            out_arc << reduce_task_no << reduce_path;
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_config__) */
