//
//  job_state.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__job_state__
#define __Twister_cpp__job_state__

#include <iostream>
#include <atomic>

namespace twister {
    enum job_exec_state { INIT = -1,
        JOB_CONFIGED = 0, MAP_TASK_CONFIGED = 1, MAP_TASK_STARTED = 2, \
        MAPPER_STARTED = 3, MAPPER_COMPLETED = 4, SHUFFLE_STARTED = 5, SHUFFLE_COMPLETED = 6, \
        REDUCE_TASK_CONFIGED = 7, REDUCE_TASK_STARTED = 8, REDUCER_STARTED = 9, REDUCER_COMPLETED = 10, \
        GATHER_STARTED = 11, GATHER_COMPLETED = 12, JOB_COMPLETED = 13};
    
    class job_state {
    private:
        static job_state* real_instance;
        
    private:
        job_state () {
            num_of_successful_map_tasks = 0;
            job_running_state = job_exec_state::INIT;
        }
        
    public:
        ~job_state() {}
        
    public:
        static std::atomic<int> num_of_successful_map_tasks;
        static std::atomic<job_exec_state> job_running_state;
        
    public:
        static void set_job_running_state(job_exec_state state_) {
            job_running_state = state_;
        }
        
        static job_state* getInstance() {
            if (real_instance == NULL) {
                real_instance = new job_state;
            }
            
            return real_instance;
        }
    };
}

#endif /* defined(__Twister_cpp__job_state__) */
