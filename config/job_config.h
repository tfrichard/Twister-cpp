//
//  job_config.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__job_config__
#define __Twister_cpp__job_config__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Twister-cpp/log/log.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class boost::uuids::uuid;

namespace twister{
    class job_config{
    public:
        std::string job_name;
        boost::uuids::uuid job_id;
        unsigned int num_mappers;
        unsigned int num_reducers;
        std::map<unsigned int, std::string> daemon_map;
    
    private:
        //log4cxx::LoggerPtr logger;
        static job_config *instance;
        
    private:
        job_config(std::string job_name = "");
        virtual ~job_config();
        
    public:
        static job_config* getInstance();
        void release();
    };
    
    
}
#endif /* defined(__Twister_cpp__job_config__) */
