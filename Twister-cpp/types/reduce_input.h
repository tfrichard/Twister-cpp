//
//  reduce_input.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/14/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reduce_input__
#define __Twister_cpp__reduce_input__

#include <iostream>
#include <map>
#include <vector>

#include <Twister-cpp/message/message.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>
#include <Twister-cpp/utility/class_factory.h>

namespace twister {
    class reduce_input : public message {
    public:
        std::map<const key*, std::vector<const value*>> key_values;
        std::map<int, const key*> hash_key_map;
        std::string key_class;
        std::string value_class;
        
    public:
        reduce_input() {
            msg_type = MSG_TYPE::REDUCE_INPUT;
            key_class = "";
            value_class = "";
        }
        
        reduce_input(const std::string &key_class_, const std::string &value_class_) {
            msg_type = MSG_TYPE::REDUCE_INPUT;
            key_class = key_class_;
            value_class = value_class_;
        }
        
        reduce_input(in_archive& in_ar) {
            load(in_ar);
        }
        
        void add_key_value(const key* key_, const value* val_) {
            if (hash_key_map.find(key_->hash_code()) == hash_key_map.end()) {
                hash_key_map[key_->hash_code()] = key_;
                key_values[key_] = std::vector<const value*>();
                key_values[key_].push_back(val_);
            } else {
                const key* u_key = hash_key_map[key_->hash_code()];
                key_values[u_key].push_back(val_);
            }
        }

		void merge_reduce_input(const reduce_input& reduce_input_) {
			std::map<const key*, std::vector<const value*>>::const_iterator it = reduce_input_.key_values.begin();

			for (; it != reduce_input_.key_values.end(); it++) {
				for (const value* val : it->second) {
					add_key_value(it->first, val);
				}
			}
		}
        
        void load(in_archive& in_ar) {
            in_ar >> (int&)msg_type;
            int num_of_keys = 0;
            in_ar >> (int&)num_of_keys;
            in_ar >> key_class;
            in_ar >> value_class;
            
            key_t_ptr* key_ptr = (key_t_ptr*)class_factory::get_instance(key_class);
            value_t_ptr* value_ptr = (value_t_ptr*)class_factory::get_instance(value_class);
         
            for (int i = 0; i < num_of_keys; i++) {
                key* real_key = key_ptr();
                in_ar >> *real_key;
                int num_of_values = 0;
                in_ar >> (int&)num_of_values;
                std::vector<const value*> values;
                for (int j = 0; j < num_of_values; j++) {
                    value* real_value = value_ptr();
                    in_ar >> *real_value;
                    values.push_back(real_value);
                }
                key_values[real_key] = values;
            }
        }
        
        void save(out_archive& out_ar) const {
            out_ar << msg_type;
            int num_of_keys = key_values.size();
            out_ar << num_of_keys;
            
            std::map<const key*, std::vector<const value*>>::const_iterator it = key_values.begin();
            if (it != key_values.end()) {
                out_ar << it->first->get_class_name();
                out_ar << it->second[0]->get_class_name();
            }
            
            for (; it != key_values.end(); it++) {
                out_ar << *it->first;
                int num_of_values = it->second.size();
                out_ar << num_of_values;
                for (int i = 0; i < it->second.size(); i++) {
                    out_ar << *it->second[i];
                }
            }
        }
    };
}

#endif /* defined(__Twister_cpp__reduce_input__) */
