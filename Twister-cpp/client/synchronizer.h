//
//  synchronizer.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__synchronizer__
#define __Twister_cpp__synchronizer__

#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <atomic>

#include <Twister-cpp/config/job_state.h>
#include <Twister-cpp/config/job_config.h>

namespace twister {
    class driver;
    
    class synchronizer {
    public:
        driver *driver_ptr;
        job_config *job_conf;
        job_state *job_state_ptr;
        
        //map between daemon no and daemon ip
        std::unordered_map<unsigned int, std::string> daemon_info;
        
        std::atomic<bool> is_sync_started;
        
        //all types of performance gauge
        long map_min_exec_time;
        long map_max_exec_time;
        long map_avg_exec_time;
        
    public:
        synchronizer();
        synchronizer(driver*, job_config*, job_state*);
        ~synchronizer();
        
    public:
        //main function to statically synchronize map_reduce job
        void sync_job_running();
        bool main_sync_loop();
    };
}


#endif /* defined(__Twister_cpp__synchronizer__) */
