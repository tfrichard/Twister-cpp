//
//  map_output_collector.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__map_output_collector__
#define __Twister_cpp__map_output_collector__

#include <iostream>
#include <unordered_map>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>

namespace twister {
    class map_output_collector {
    public:
        void collect(const key&, const value&);
        
    private:
        //a stub for key value container
        std::unordered_map<std::string, std::string> container;
    };
}
#endif /* defined(__Twister_cpp__map_output_collector__) */
