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

#include <Twister-cpp/log/log.h>
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/config/job_state.h>
#include <Twister-cpp/communication/conn_manager.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <utility>
#include <vector>

#endif /* defined(__Twister_cpp__driver__) */

namespace twister {
    class driver {
    private:
        //log4cxx::LoggerPtr log;
        job_config *jobconf;
        job_state *jobstate;
        conn_manager *conn_mgr;
        
    public:
        driver();
        virtual ~driver();
        
    public:
        bool runMapReduceJob(std::vector<std::pair<key, value>>);
        
    private:
        void configMapTasks();
        void test_conn(std::vector<std::string>);
    };
}
