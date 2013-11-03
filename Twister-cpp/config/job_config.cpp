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
    
    job_config::job_config() {
        this->job_name = "";
        this->mapper_name = "";
        this->num_mappers = 0;
        this->num_reducers =0;
        this->reducer_name = "";
        this->udf_bin_path = "";
        this->reduce_path = "";
        this->iteration = 0;
        this->gather_name = "";
        this->udf_dy_lib = "";
    }
    
    job_config& job_config::operator=(const twister::job_config &job_conf) {
        this->job_name = job_conf.job_name;
        this->mapper_name = job_conf.mapper_name;
        this->num_mappers = job_conf.num_mappers;
        this->num_reducers = job_conf.num_reducers;
        this->reducer_name = job_conf.reducer_name;
        this->udf_bin_path = job_conf.udf_bin_path;
        this->reduce_path = job_conf.reduce_path;
        this->iteration = job_conf.iteration;
        this->gather_name = job_conf.gather_name;
        this->daemon_map = job_conf.daemon_map;
        this->partition_path = job_conf.partition_path;
        this->udf_dy_lib = job_conf.udf_dy_lib;
        return *this;
    }
    
    job_config::job_config(const job_config& job_conf) {
        *this = job_conf;
    }
    
    job_config::job_config(std::string job_name_) {
        //LOG4CXX_INFO(job_config::logger, "start job config")
        std::cout << "job_config starts" << std::endl;
        
        job_name = job_name_;
        iteration = 0;
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
        
#ifdef _DEBUG
        udf_bin_path = "/Users/feiteng/Library/Developer/Xcode/DerivedData/Twister-cpp-bhxrgrfnxsvvhwfeqbijayxvhjpd/Build/Products/Debug/";
        reduce_path = udf_bin_path;
        mapper_name = "word_count_mapper";
        reducer_name = "word_count_reducer";
        gather_name = "word_count_gather";
#endif
        
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
        
        std::string partition_file(twister_home);
        partition_file += "/data/wc_data";
        std::cout << "data file: " << partition_file << std::endl;
        
        std::ifstream partition_list(partition_file, std::ifstream::in);
        std::string data_file = "";
        while (getline(partition_list, data_file)) {
            partition_path.push_back(data_file);
        }
        
        std::cout << "job_config ends" << std::endl;
    }
    
    job_config* job_config::getInstance(const std::string &job_name) {
        if (instance == NULL) {
            instance = new job_config(job_name);
        }
        return instance;
    }
    
    job_config* job_config::getInstance() {
        if (instance == NULL) {
            instance = new job_config();
        }
        return instance;
    }
    
    void job_config::load(in_archive &in_ac) {
        in_ac >> job_name;
        in_ac >> mapper_name;
        in_ac >> reducer_name;
        in_ac >> gather_name;
        in_ac >> udf_bin_path;
        in_ac >> reduce_path;
        in_ac >> udf_dy_lib;
        in_ac >> (int&)iteration;
        in_ac >> (int&)num_mappers;
        in_ac >> (int&)num_reducers;
        int num_daemons;
        in_ac >> (int&)num_daemons;
        for (int i = 0; i < num_daemons; i++) {
            std::pair<int, std::string> tmp;
            in_ac >> (int&)tmp.first >> tmp.second;
            daemon_map.insert(tmp);
        }
        int num_partitions;
        in_ac >> (int&)num_partitions;
        for (int i = 0; i < num_partitions; i++) {
            std::string tmp;
            in_ac >> tmp;
            partition_path.push_back(tmp);
        }
    }
    
    void job_config::save(twister::out_archive &out_ac) const {
        out_ac << job_name;
        out_ac << mapper_name;
        out_ac << reducer_name;
        out_ac << gather_name;
        out_ac << udf_bin_path;
        out_ac << reduce_path;
        out_ac << udf_dy_lib;
        out_ac << iteration;
        out_ac << num_mappers;
        out_ac << num_reducers;
        int num_daemons = (int)daemon_map.size();
        out_ac << num_daemons;
        for (std::map<int, std::string>::const_iterator it = daemon_map.begin(); it != daemon_map.end(); it++) {
            out_ac << it->first << it->second;
        }
        int num_partitions = (int)partition_path.size();
        out_ac << num_partitions;
        for (int i = 0; i < num_partitions; i++) {
            out_ac << partition_path[i];
        }

    }
    
    job_config::~job_config(){}
}
