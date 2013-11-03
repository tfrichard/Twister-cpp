//
//  string_key.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/28/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__string_key__
#define __Twister_cpp__string_key__

#include <iostream>
#include <string>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

namespace twister {
    class string_key : public key {
    public:
        std::string key_str;
        
    public:
        string_key(const std::string& str) {key_str = str;}
        string_key() {key_str = "";}
        
        void load(in_archive& in_arc) {
            in_arc >> key_str;
        }

        void save(out_archive& out_arc) const {
            out_arc << key_str;
        }

        bool operator<(const string_key& key_) const {
            return key_str < key_.key_str;
        }
        
        virtual std::string get_class_name() const {
            return "string_key";
        }
        
        virtual int hash_code() const {
            std::hash<std::string> str_hash_func;
            return abs(str_hash_func(key_str));
        }
    };
}

#endif /* defined(__Twister_cpp__string_key__) */
