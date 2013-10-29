//
//  gather.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/24/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__gather__
#define __Twister_cpp__gather__

#include <iostream>
#include <map>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/config/job_config.h>

namespace twister {
    class gather {
    public:
        virtual void do_gather(std::map<const key*, const value*>&) = 0;
        virtual void configure(const job_config*) = 0;
        virtual void close() = 0;
    };
    
    typedef gather* gather_t_ptr();
}

#endif /* defined(__Twister_cpp__gather__) */
