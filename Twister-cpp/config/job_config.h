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
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class boost::uuids::uuid;

namespace twister{
    class job_config : public message {
    public:
        std::string job_name;
        //boost::uuids::uuid job_id;
        int iteration;
        int num_mappers;
        int num_reducers;
        std::map<int, std::string> daemon_map;
        std::string mapper_name;
        std::string reducer_name;
        std::string gather_name;
        std::vector<std::string> partition_path;
        std::string udf_bin_path;
        std::string reduce_path;
        std::string udf_dy_lib;
    
    private:
        //log4cxx::LoggerPtr logger;
        static job_config *instance;
        
    public:
        job_config();
        job_config(std::string job_name);
        job_config(const job_config&);
        job_config& operator=(const job_config&);
        virtual ~job_config();
        
    public:
        static job_config* getInstance(const std::string&);
        static job_config* getInstance();
        void release();
        
        void load(in_archive&);
        void save(out_archive&) const;
    };
    
    
}
#endif /* defined(__Twister_cpp__job_config__) */
