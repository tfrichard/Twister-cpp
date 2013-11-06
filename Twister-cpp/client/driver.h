//
//  driver.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__driver__
#define __Twister_cpp__driver__

#include <iostream>

//#include <Twister-cpp/log/log.h>
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/config/job_state.h>
#include <Twister-cpp/communication/conn_manager.h>
#include <Twister-cpp/client/synchronizer.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/worker/gather.h>
#include <utility>
#include <vector>

#endif /* defined(__Twister_cpp__driver__) */

namespace twister {
    class synchronizer;
    
    class driver {
    private:
        //log4cxx::LoggerPtr log;
        job_config *jobconf;
        job_state *jobstate;
        conn_manager *conn_mgr;
        synchronizer *sync_mgr;
        gather* gather_;
        
    public:
        driver();
        driver(job_config*);
        virtual ~driver();
        
    public:
        bool runMapReduceJob(std::vector<std::pair<key, value>>);
        bool runMapReduceJob(value*);
        
        bool is_mapreduce_completed();
        inline bool is_has_reduce() const { return ! jobconf->num_reducers == 0; }
        void start_shuffle();
        void start_reduce();
        void start_map(value*);
        void start_gather();
        gather* get_gather() { return gather_; }
        
    private:
        void config_map_tasks();
        void config_reduce_tasks_bcast();
        void config_reduce_tasks();
        void config_reduce_inputs();
        void test_conn(std::vector<std::string>);
        void on_event();
        void receive_gather_input(char*, int);
        
        //message& wait_for_ack();
    };
}
