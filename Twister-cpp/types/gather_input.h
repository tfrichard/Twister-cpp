//
//  gather_input.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/24/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__gather_input__
#define __Twister_cpp__gather_input__

#include <iostream>
#include <map>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/utility/class_factory.h>

namespace twister {
    class gather_input : public message {
    public:
        int task_no;
        int iteration;
        std::string key_class;
        std::string value_class;
        std::map<const key*, const value*> key_value_output;
        
        gather_input() {
            msg_type = MSG_TYPE::GATHER_INPUT;
            task_no = 0;
            iteration = 0;
            key_class = "";
            value_class = "";
        }
        
        gather_input(int task_no_, int iteration_, const std::string key_class_, const std::string value_class_)
        : task_no(task_no_), iteration(iteration_), key_class(key_class_), value_class(value_class_)
        {
            msg_type = MSG_TYPE::GATHER_INPUT;
        }
        
        gather_input(in_archive& in_ar) {
            load(in_ar);
        }
        
        void add_key_value(const key* key_, const value* val_) {
            if (key_class == "" || value_class == "") {
                key_class = key_->get_class_name();
                value_class = val_->get_class_name();
            }
            
            if (key_value_output.find(key_) == key_value_output.end()) {
                key_value_output[key_] = val_;
            }
            
            key_value_output[key_] = val_;
        }
        
        void load(in_archive& in_ar) {
            in_ar >> (int&)msg_type;
            in_ar >> (int&)task_no;
            in_ar >> (int&)iteration;
            in_ar >> key_class;
            in_ar >> value_class;
            
            int num_key_values = 0;
            in_ar >> (int&)num_key_values;
            
            key_t_ptr* key_ptr = (key_t_ptr*)class_factory::get_instance(key_class);
            value_t_ptr* value_ptr = (value_t_ptr*)class_factory::get_instance(value_class);
            
            for (int i = 0; i < num_key_values; i++) {
                key* real_key = key_ptr();
                value* real_val = value_ptr();
                in_ar >> *real_key;
                in_ar >> *real_val;
                
                add_key_value(real_key, real_val);
            }
        }
        
        void save(out_archive& out_ar) const {
            out_ar << msg_type;
            out_ar << task_no;
            out_ar << iteration;
            out_ar << key_class;
            out_ar << value_class;
            
            int num_key_values = key_value_output.size();
            out_ar << num_key_values;
            
            std::map<const key*, const value*>::const_iterator it = key_value_output.begin();
            
            for (; it != key_value_output.end(); it++) {
                out_ar << *it->first;
                out_ar << *it->second;
            }
        }
    };
}

#endif /* defined(__Twister_cpp__gather_input__) */
