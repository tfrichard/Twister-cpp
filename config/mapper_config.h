//
//  mapper_config.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/7/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__mapper_config__
#define __Twister_cpp__mapper_config__

#include <iostream>
#include <string>

#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>
#include <Twister-cpp/message/message.h>

namespace twister {
    class mapper_config : public message {
    public:
        int map_task_no;
        std::string partition_path;
        std::string map_path;
        
        mapper_config() {}
        
        mapper_config(unsigned int task_no, std::string partition_path_, std::string map_path_) : map_task_no(task_no), partition_path(partition_path_), map_path(map_path_) {
        }
        
        mapper_config(const mapper_config& other) {
            map_task_no = other.map_task_no;
            partition_path = other.partition_path;
            map_path = other.map_path;
        }
        
        mapper_config& operator=(const mapper_config& other) {
            map_task_no = other.map_task_no;
            partition_path = other.partition_path;
            map_path = other.map_path;
            
            return *this;
        }
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)map_task_no >> partition_path >> map_path;
        }
        
        void save(out_archive& out_arc) const {
            out_arc << map_task_no << partition_path << map_path;
        }
    };
}

#endif /* defined(__Twister_cpp__mapper_config__) */
