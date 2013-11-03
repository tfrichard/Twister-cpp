//
//  synchronizer.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <thread>

#include <Twister-cpp/client/synchronizer.h>
#include <Twister-cpp/client/driver.h>

namespace twister {
    synchronizer::synchronizer() {
        is_sync_started = false;
    }
    
    synchronizer::synchronizer(driver* driver_, job_config* job_conf_, job_state* job_state_)
    : driver_ptr(driver_), job_conf(job_conf_), job_state_ptr(job_state_) {
        is_sync_started = true;
    }
    
    synchronizer::~synchronizer() {}
    
    bool synchronizer::main_sync_loop() {
        if (!is_sync_started) {
            return false;
        }
        
        std::cout << "sync_manager starting from here" << std::endl;
        
        while (!driver_ptr->is_mapreduce_completed()) {
            //sleep time consistent with Twister java
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            sync_job_running();
        }
        
        std::cout << "synchronizer is completed!" << std::endl;
        return true;
    }
    
    void synchronizer::sync_job_running() {
        
        //std::cout << "sync job running starting from here" << std::endl;
        
        if (job_state_ptr->job_running_state == job_exec_state::MAPPER_COMPLETED) {
            driver_ptr->start_shuffle();
        }
        
        if (job_state_ptr->job_running_state == SHUFFLE_COMPLETED) {
            //may need reduceInput as parameter of start_reduce()
            driver_ptr->start_reduce();
        }
        
        if (job_state_ptr->job_running_state == REDUCER_COMPLETED) {
            driver_ptr->start_gather();
        }
    }
}
