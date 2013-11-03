//
//  reduce_output_collector.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_output_collector__
#define __Twister_cpp__reduce_output_collector__

#include <iostream>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/gather_input.h>

namespace twister {
    class reduce_output_collector{
    public:
        int iteration;
        int task_no;
        gather_input gather_input_;
        
    public:
        reduce_output_collector() {
            iteration = -1;
            task_no = -1;
        }
        
        reduce_output_collector(int iter, int task_no_) : iteration(iter), task_no(task_no_) {
            gather_input_.task_no = task_no;
            gather_input_.iteration = iteration;
        }
        
        void collect(const key* key_, const value* value_) {
            //container[key_] = value_;
            gather_input_.add_key_value(key_, value_);
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_output_collector__) */
