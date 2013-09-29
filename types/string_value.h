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
    };
}

#endif /* defined(__Twister_cpp__string_value__) */
