//
//  int_value.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 10/14/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/types/int_value.h>
//dynamic linking needs it as hooker
extern "C" {
    void* create_int_value_instance() { return new twister::int_value; }
}