//
//  word_count_reducer.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 10/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "word_count_reducer.h"
#include <Twister-cpp/types/string_key.h>
#include <Twister-cpp/types/int_value.h>

void word_count_reducer::reduce(twister::reduce_output_collector *collector, const twister::key* key, const std::vector<const twister::value*> &values) {
    std::string str = static_cast<const twister::string_key&>(*key).key_str;
    std::cout << "string key is: " << str << std::endl;
    
    int sum = 0;
    for (const twister::value* val : values) {
        sum += static_cast<const twister::int_value&>(*val).value_int;
    }
    
    collector->collect(new twister::string_key(str), new twister::int_value(sum));
}

void word_count_reducer::configure(const twister::job_config *jobconf, const twister::reducer_config *reduce_conf) {
    std::cout << "config in wc reducer" << std::endl;
}

void word_count_reducer::configure(const twister::job_config *jobconf) {}

void word_count_reducer::close() {}

//this non-member function is necessary for every derived mapper class
//dynamic linking needs it as hooker
extern "C" {
    void* create_word_count_reducer_instance() { return new word_count_reducer; }
}