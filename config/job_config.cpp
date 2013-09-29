//
//  job_config.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <string>

#include <Twister-cpp/config/job_config.h>
#include <cstdlib>
#include <malloc/malloc.h>

namespace twister {
    //log4cxx::LoggerPtr job_config::logger = log::get_logger("job_config");
    job_config* job_config::instance = NULL;
    
    job_config::job_config(std::string job_name_) {
        //LOG4CXX_INFO(job_config::logger, "start job config")
        std::cout << "job_config starts" << std::endl;
        
        job_id = boost::uuids::random_generator()();
        job_name = job_name_;
        instance = NULL;
        char* twister_home = NULL;
        if ((twister_home = getenv("TWISTER_HOME")) == NULL) {
            //LOG4CXX_ERROR(job_config::logger, "No TWISTER_HOME env. \
                          Please set it properly before starting the job");
            std::cerr << "Please set TWISTER_HOME!" << std::endl;
#ifdef _DEBUG
            twister_home = (char*)malloc(sizeof(char) * 256);
            strcpy(twister_home, "/Users/feiteng/Documents/xcode-projectes/Twister-cpp/Twister-cpp");
#else
            exit(1);
#endif
        }
        
        std::string node_file(twister_home);
        node_file += "/bin/nodes";
        std::cout << "node file: " << node_file << std::endl;
        
        std::ifstream node_list(node_file, std::ifstream::in);
        
        std::string ip_str = "";
        unsigned int i = 0;
        while (getline(node_list, ip_str)) {
            daemon_map[i] = ip_str;
        }
        
        node_list.close();
        
        std::cout << "job_config ends" << std::endl;
    }
    
    job_config* job_config::getInstance() {
        if (instance == NULL) {
            instance = new job_config();
        }
        return instance;
    }
    
    job_config::~job_config(){}
}
