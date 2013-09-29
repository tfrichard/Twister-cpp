//
//  word_count_mapper.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "word_count_mapper.h"
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/utility/map_factory.h>
#include <Twister-cpp/types/string_key.h>
#include <Twister-cpp/types/string_value.h>
#include <iostream>

void word_count_mapper::map(map_output_collector *map_output_collect, const twister::key& key_, const twister::value& value_) {
    map_output_collect->collect(twister::string_key("key"), twister::string_value("value"));
    std::cout << "here's word_count_map function" << std::endl;
}

void word_count_mapper::configure(const twister::job_config & job_conf) {
    std::cout << "the mapper of job: " << job_conf.job_name << std::endl;
}

void word_count_mapper::close() {
    
}

word_count_mapper::~word_count_mapper() {}


