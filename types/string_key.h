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

namespace twister {
    class string_key : public key {
    public:
        std::string key_str;
        
    public:
        string_key(const std::string& str) {key_str = str;}
        string_key() {key_str = "";}
    };
}

#endif /* defined(__Twister_cpp__string_key__) */
