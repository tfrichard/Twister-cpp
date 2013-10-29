//
//  string_value.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/28/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__string_value__
#define __Twister_cpp__string_value__

#include <iostream>
#include <string>
#include <Twister-cpp/types/value.h>

namespace twister {
    class string_value : public value {
    public:
        std::string value_str;
        
    public:
        string_value() {value_str = "";}
        string_value(const std::string& str) {value_str = str;}
        
        void load(in_archive& in_arc) {
            in_arc >> value_str;
        }
        
        void save(out_archive& out_arc) const {
            out_arc << value_str;
        }
        
        virtual std::string get_class_name() const {
            return "string_value";
        }
    };
}

#endif /* defined(__Twister_cpp__string_value__) */
