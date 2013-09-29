//
//  mapper.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__mapper__
#define __Twister_cpp__mapper__

#include <iostream>
#include <Twister-cpp/worker/map_output_collector.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/config/job_config.h>

namespace twister {
    class mapper {
    public:
        virtual void map(map_output_collector*, const key&, const value&) = 0;
        virtual void configure(const job_config&) = 0;
        virtual void close() = 0;
        virtual ~mapper() {};
    };
}

#endif /* defined(__Twister_cpp__mapper__) */
