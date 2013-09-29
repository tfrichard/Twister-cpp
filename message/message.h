//
//  message.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/26/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__message__
#define __Twister_cpp__message__

#include <iostream>

namespace twister {
    class message {
        //enum {start_map_task = 0};
    public:
        enum {head_length = 4};
        enum {max_body_length = 1024 * 1024};
        
    };
}

#endif /* defined(__Twister_cpp__request__) */
