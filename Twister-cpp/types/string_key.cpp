//
//  string_key.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/28/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/types/string_key.h>

//this non-member function is necessary for every derived key class
//dynamic linking needs it as hooker
extern "C" {
    void* create_string_key_instance() { return new twister::string_key; }
}
