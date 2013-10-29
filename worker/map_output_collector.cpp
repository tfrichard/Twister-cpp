//
//  map_output_collector.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/worker/map_output_collector.h>
#include <Twister-cpp/utility/reduce_selector.h>

namespace twister {
    void map_output_collector::collect(const key *key_, const value *value_) {
        reduce_selector r_selector(job_conf->num_reducers, job_conf->job_name);
        int reduce_no = r_selector.get_reducer(*key_);
        
        if (reduce_input_table.find(reduce_no) == reduce_input_table.end()) {
            reduce_input_table[reduce_no] = reduce_input(key_->get_class_name(), value_->get_class_name());
        }
        
        std::cout << "add key values into reduce input table" << std::endl;
        reduce_input_table[reduce_no].add_key_value(key_, value_);
        
        if (reduce_input_cnt_table.find(reduce_no) == reduce_input_cnt_table.end()) {
            reduce_input_cnt_table[reduce_no] = 0;
        }
        
        reduce_input_cnt_table[reduce_no] += 1;
    }
}