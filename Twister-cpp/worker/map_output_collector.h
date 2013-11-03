//
//  map_output_collector.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_output_collector__
#define __Twister_cpp__map_output_collector__

#include <iostream>
#include <map>

#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/reduce_input.h>
#include <Twister-cpp/config/job_config.h>

namespace twister {
    class map_output_collector {
    public:
        //<reduce_no, reduce_input>
        std::map<int, reduce_input> reduce_input_table;
        std::map<int, int> reduce_input_cnt_table;
        int iteration;
        job_config *job_conf;
        
    public:
        map_output_collector() {
            iteration = 0;
            job_conf = NULL;
        }
        
        map_output_collector(job_config* job_conf_) : job_conf(job_conf_) {
            iteration = job_conf_->iteration;
            reduce_input_table = std::map<int, reduce_input>();
        }
        
        ~map_output_collector() {}
        
        void collect(const key*, const value*);
    };
}
#endif /* defined(__Twister_cpp__map_output_collector__) */
