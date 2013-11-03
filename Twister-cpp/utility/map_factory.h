//
//  map_factory.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/28/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_factory__
#define __Twister_cpp__map_factory__

#include <iostream>
#include <utility>
#include <Twister-cpp/worker/mapper.h>

namespace twister {
    template <typename T>
    mapper* create_mapper() {
        return new T;
    }
    
    typedef std::map<std::string, mapper*(*)()> mapper_type;
    
    struct map_factory {
    public:
        static mapper* create_mapper_instance(std::string mapper_name) {
            mapper_type::iterator it = get_reg_map()->find(mapper_name);
            if (it == get_reg_map()->end()) return NULL;
            return it->second();
        }
        
        static void registe_map(std::string mapper_name) {
            if (get_reg_map()->find(mapper_name) == get_reg_map()->end()) {
                
            }
        }
        
        static mapper_type* get_reg_map() {
            if (!reg_map) reg_map = new mapper_type;
            return reg_map;
        }

    private:
        static mapper_type *reg_map;
    };
    
    template<typename T>
    void registe_map(std::string mapper_name) {
        if (map_factory::get_reg_map()->find(mapper_name) == map_factory::get_reg_map()->end()) {
            map_factory::get_reg_map()->insert(std::make_pair(mapper_name, &create_mapper<T>));
        }
    }
    
}

#endif /* defined(__Twister_cpp__map_factory__) */
