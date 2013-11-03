//
//  map_start_msg.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/10/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_start_msg__
#define __Twister_cpp__map_start_msg__

#include <iostream>
#include <map>

#include <Twister-cpp/message/message.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/utility/class_factory.h>

namespace twister {
    class map_start_msg : public message {
    public:
        std::map<const key*, const value*> key_value_table;
        std::string job_name;
        int map_task_no;
        std::string key_class;
        std::string value_class;
        
    public:
        map_start_msg() {
            msg_type = MSG_TYPE::START_MAPPER;
            map_task_no = -1;
            job_name = "";
            key_class = "";
            value_class = "";
        }
        
        map_start_msg(std::string &job_name_, int map_task_no_, std::string key_class_, std::string val_class_) : job_name(job_name_), map_task_no(map_task_no_), key_class(key_class_), value_class(val_class_) {
            msg_type = MSG_TYPE::START_MAPPER;
        }
        
        map_start_msg(in_archive &in_arc) {
            load(in_arc);
        }
        
        void add_key_values(const key* key_, const value* val_) {
            key_value_table[key_] = val_;
        }
        
        ~map_start_msg() throw() {}
    
        void load(in_archive& in_arc) {
            in_arc >> (int&)msg_type;
            in_arc >> job_name;
            in_arc >> (int&)map_task_no;
            int num_of_key_values;
            in_arc >> (int&)num_of_key_values;
            in_arc >> key_class;
            in_arc >> value_class;
            
            key_t_ptr* key_ptr = (key_t_ptr*)class_factory::get_instance(key_class);
            value_t_ptr* value_ptr = (value_t_ptr*)class_factory::get_instance(value_class);
            
            for (int i = 0; i < num_of_key_values; i++) {
                key* real_key = key_ptr();
                value* real_value = value_ptr();
                
                in_arc >> *real_key;
                in_arc >> *real_value;
                key_value_table.insert(std::make_pair(real_key, real_value));
            }
            
        }
    
        void save(out_archive& out_arc) const {
            out_arc << msg_type;
            out_arc << job_name;
            out_arc << map_task_no;
            int num_of_key_values = (int)key_value_table.size();
            out_arc << num_of_key_values;
            out_arc << key_class;
            out_arc << value_class;
            for (std::map<const key*,const value*>::const_iterator it = key_value_table.begin(); it != key_value_table.end(); it++) {
                out_arc << *it->first;
                out_arc << *it->second;
            }
        }
        
    };
}

#endif /* defined(__Twister_cpp__map_start_msg__) */
