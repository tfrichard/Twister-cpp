//
//  map_task.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/tasks/map_task.h>

namespace twister {
    void map_task::run_map_function(const map_start_msg& map_start) {
        std::cout << "here we invoke user's mapper function" << std::endl;
        
        if (collector == NULL) {
            collector = new map_output_collector(&job_conf);
        }
        
        const std::map<const key*, const value*> &key_values = map_start.key_value_table;
        
        std::map<const key*, const value*>::const_iterator key_iter = key_values.begin();
        for (; key_iter != key_values.end(); key_iter++) {
            map_instance->map(collector, *key_iter->first, *key_iter->second);
        }
    }
}
