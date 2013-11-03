//
//  int_value.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/14/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__int_value__
#define __Twister_cpp__int_value__

#include <iostream>
#include <Twister-cpp/types/value.h>

namespace twister {
    class int_value : public value {
    public:
        int value_int;
        
    public:
        int_value() {value_int = 0;}
        int_value(int value_int_) { value_int = value_int_; }
        
        void load(in_archive& in_arc) {
            in_arc >> (int&)value_int;
        }
        
        void save(out_archive& out_arc) const {
            out_arc << value_int;
        }
        
        virtual std::string get_class_name() const {
            return "int_value";
        }
    };

}

#endif /* defined(__Twister_cpp__int_value__) */
