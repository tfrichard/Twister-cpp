//
//  log.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef Twister_cpp_log_h
#define Twister_cpp_log_h

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <sys/stat.h>
#include <map>

namespace twister {
    /*
    class log {
    public:
        static void init();        
        static log4cxx::LoggerPtr get_logger(const std::string &class_name);
        
    private:
        static std::map<std::string, log4cxx::LoggerPtr> instance_table;
        static bool is_init;
    };
    */
    
}

#endif
