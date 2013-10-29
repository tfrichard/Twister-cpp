//
//  word_count_reducer.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__word_count_reducer__
#define __Twister_cpp__word_count_reducer__

#include <iostream>
#include <vector>
#include <Twister-cpp/worker/reducer.h>
#include <Twister-cpp/config/reduce_config.h>
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>

using namespace twister;

class word_count_reducer : public reducer {
public:
    void reduce(reduce_output_collector*, const twister::key*, const std::vector<const twister::value*>&);
    void configure(const job_config*, const reducer_config*);
    void configure(const job_config*);
    void close();
    //virtual void* create_instance() = 0;
    virtual ~word_count_reducer() {};
};

#endif /* defined(__Twister_cpp__word_count_reducer__) */
