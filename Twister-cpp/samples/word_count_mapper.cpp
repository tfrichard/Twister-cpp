//
//  word_count_mapper.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "word_count_mapper.h"
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/config/mapper_config.h>
#include <Twister-cpp/utility/map_factory.h>
#include <Twister-cpp/types/string_key.h>
#include <Twister-cpp/types/string_value.h>
#include <Twister-cpp/types/int_value.h>
#include <iostream>
#include <fstream>

void word_count_mapper::map(map_output_collector *map_output_collect, const twister::key& key_, const twister::value& value_) {
    std::cout << "key's value is: ";
    std::cout << static_cast<const twister::string_key&>(key_).key_str << std::endl;
    std::cout << "value's value is:";
    std::cout << static_cast<const twister::string_value&>(value_).value_str << std::endl;
    std::map<std::string, int> wc_map;
    for (std::string &word : words) {
        if (wc_map.find(word) == wc_map.end()) {
            wc_map[word] = 0;
        }
        wc_map[word] += 1;
    }
    
    for (std::map<std::string, int>::const_iterator it = wc_map.begin(); it != wc_map.end(); it++) {
        map_output_collect->collect((new string_key(it->first)), (new int_value(it->second)));
    }

    std::cout << "here we finished word_count_map function" << std::endl;
}

void word_count_mapper::configure(const job_config *job_conf, const mapper_config *mapper_conf) {
    this->partition_path = mapper_conf->partition_path;
    
    std::cout << "the no. " << mapper_conf->map_task_no << " mapper of job: "
            << job_conf->job_name << " started"<< std::endl;
    //fill in the local vector of words
    std::ifstream in_file(partition_path.c_str());
    std::string str = "";
    while (std::getline(in_file, str, ' ')) {
        words.push_back(str);
    }
}

void word_count_mapper::configure(const job_config* mc) {}

void word_count_mapper::close() {
    
}

//this non-member function is necessary for every derived mapper class
//dynamic linking needs it as hooker
extern "C" {
void* create_word_count_mapper_instance() { return new word_count_mapper; }
}

word_count_mapper::~word_count_mapper() {}


