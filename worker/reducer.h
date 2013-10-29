//
//  reducer.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__reducer__
#define __Twister_cpp__reducer__

#include <iostream>
#include <Twister-cpp/config/reduce_config.h>
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/worker/reduce_output_collector.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>

namespace twister {
    class reducer {
    public:
        virtual void reduce(reduce_output_collector*, const key*, const std::vector<const value*>&) = 0;
        virtual void configure(const job_config*, const reducer_config*) = 0;
        virtual void configure(const job_config*) = 0;
        virtual void close() = 0;

    };
    
    typedef reducer* reducer_t_ptr();
}

#endif /* defined(__Twister_cpp__reducer__) */
