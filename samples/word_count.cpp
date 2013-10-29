//
//  word_count.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include "word_count_mapper.h"
#include <Twister-cpp/config/job_config.h>
#include <Twister-cpp/config/driver_config.h>
#include <Twister-cpp/client/driver.h>
#include <Twister-cpp/utility/map_factory.h>
#include <Twister-cpp/types/string_key.h>
#include <Twister-cpp/types/string_value.h>

#include <iostream>

//int main(int argc, char* argv[]) {
//    twister::job_config *job_conf = twister::job_config::getInstance();
//    job_conf->job_name = "word count";
//    job_conf->mapper_name = "word_count_mapper";
//    twister::driver driver(job_conf);
//
////    registe_map<word_count_mapper>("word_count_mapper");
////    mapper * base = map_factory::create_mapper_instance("word_count_mapper");
////    base->map(new twister::map_output_collector, string_key("key"), string_value("value"));
//    
//    //driver.configMapTasks();
//   
//    //delete base;
//    return 0;
//}
