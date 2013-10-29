//
//  word_count_gather.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 10/24/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "word_count_gather.h"

//this non-member function is necessary for every derived gather class
//dynamic linking needs it as hooker
extern "C" {
    void* create_word_count_gather_instance() { return new word_count_gather; }
}