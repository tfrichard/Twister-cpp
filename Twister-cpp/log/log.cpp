//
//  log.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "log.h"

namespace twister {
    /*
    std::map<std::string, log4cxx::LoggerPtr> log::instance_table = \
    (std::map<std::string, log4cxx::LoggerPtr>());
    
    bool log::is_init = false;
    
    void log::init() {
        is_init = true;
        log4cxx::LoggerPtr self_logger = get_logger("log");
        if (getenv("TWISTER_HOME") == NULL) {
            LOG4CXX_ERROR(self_logger, "please set up TWISTER_HOME");
            //exit(1);
        } else {
            std::string property_path = std::string(getenv("TWISTER_HOME")) + "/bin/log_properties";
            struct stat buffer;
            if (stat(property_path.c_str(), &buffer) == 0)
                log4cxx::PropertyConfigurator::configure(property_path);
            else
                log4cxx::BasicConfigurator::configure();
        }
        
    }
    
    log4cxx::LoggerPtr log::get_logger(const std::string &class_name) {
        if (!is_init) init();
            
        if (instance_table.find(class_name) == instance_table.end()) {
            instance_table.insert(std::pair<std::string, log4cxx::LoggerPtr>(class_name, log4cxx::Logger::getLogger(class_name)));
            //instance_table[class_name] = true;
        }
        
        return instance_table[class_name];
        //return log4cxx::Logger::getLogger(class_name);
    }
     */

}
