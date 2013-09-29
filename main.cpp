//
//  main.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/11/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

//#include "log4cxx/logger.h"
//#include "log4cxx/basicconfigurator.h"
//#include "log4cxx/propertyconfigurator.h"
//#include "log4cxx/helpers/exception.h"
#include "boost/thread.hpp"
#include <Twister-cpp/client/driver.h>
#include <Twister-cpp/log/log.h>
#include <pthread.h>
#include <map>
#include <thread>

using namespace log4cxx;
using namespace log4cxx::helpers;
// Define a static logger variable so that it references the
// Logger instance named "MyApp".
//using namespace twister;
//using namespace std;

void foo() {std::cout << "in thread" << std::endl;}

int main(int argc, char **argv)
{
    /*
    LoggerPtr logger(Logger::getLogger("rootAppender"));
    LoggerPtr logger1(Logger::getLogger("testLog"));
    //LoggerPtr log(Logger::getLogger("test"));
    
    int result = EXIT_SUCCESS;
    try
    {
        if (argc > 1)
        {
            // BasicConfigurator replaced with PropertyConfigurator.
            //PropertyConfigurator::configure(argv[1]);
            BasicConfigurator::configure();
        }
        else
        {
            BasicConfigurator::configure();
        }
        
        LOG4CXX_INFO(logger, "Entering application.")
        //Bar bar
        //bar.doIt();
        
		LOG4CXX_FATAL(logger1, "test log print");
        LOG4CXX_INFO(logger, "Exiting application.")
    }
    catch(Exception&)
    {
        result = EXIT_FAILURE;
    }
     */
    
    twister::driver td;
    std::vector<std::pair<twister::key, twister::value>> key_value_input;
    td.runMapReduceJob(key_value_input);
    
    //std::thread t1(foo);
    //t1.join();
    //map<string, int> m;
    //m["fei"] = 1;
    
    //LoggerPtr mlog = twister::log::get_logger("main");
    //LOG4CXX_INFO(mlog, "main exit");
    return 1;
}

