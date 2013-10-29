//
//  word_count_mapper.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__word_count_mapper__
#define __Twister_cpp__word_count_mapper__

#include <iostream>
#include <vector>
#include <string>
#include <Twister-cpp/worker/mapper.h>

using namespace twister;

class word_count_mapper : public mapper {
public:
    void map(map_output_collector *map_output_collect, const key& key, const value& value);
    void configure(const job_config*, const mapper_config*);
    void configure(const job_config*);
    void close();
    //void* create_instance();
    virtual ~word_count_mapper();
    
private:
    std::string partition_path;
    std::vector<std::string> words;
};

#endif /* defined(__Twister_cpp__word_count_mapper__) */
