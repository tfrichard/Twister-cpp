//
//  job_state.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "job_state.h"

namespace twister {
    job_state* job_state::real_instance = NULL;
    std::atomic<int> job_state::num_of_successful_map_tasks(0);
    std::atomic<job_exec_state> job_state::job_running_state(INIT);
    
}
