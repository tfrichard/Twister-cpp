//
//  map_output_collector.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "map_output_collector.h"
#include <utility>

namespace twister {
    void map_output_collector::collect(const key &key_, const value &value_) {
        container.emplace("k", "v");
    }
}