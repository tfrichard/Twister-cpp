//
//  reduce_selector.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/14/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_selector__
#define __Twister_cpp__reduce_selector__

#include <iostream>
#include <string>
#include <functional>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/config/job_config.h>

namespace twister {
    class reduce_selector {
    private:
        int num_of_reducers;
        std::string job_name;
        
    public:
        reduce_selector(int num_, const std::string &name_) : num_of_reducers(num_), job_name(name_) {}

        int get_reducer(const key& key_) {
            return key_.hash_code() % num_of_reducers;
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_selector__) */
