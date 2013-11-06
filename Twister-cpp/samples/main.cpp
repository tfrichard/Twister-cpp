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
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include "Twister-cpp/client/driver.h"
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/string_value.h>
#include "word_count_gather.h"
#include <map>
#include <thread>
#include <fstream>

// Define a static logger variable so that it references the
// Logger instance named "MyApp".
//using namespace twister;
//using namespace std;

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void gen_words() {
    std::ofstream out_file;
    std::vector<std::string> files;
    files.push_back("/tmp/work/feiteng/data/wc/wc_0");
    files.push_back("/tmp/work/feiteng/data/wc/wc_1");
    files.push_back("/tmp/work/feiteng/data/wc/wc_2");
    files.push_back("/tmp/work/feiteng/data/wc/wc_3");
    files.push_back("/tmp/work/feiteng/data/wc/wc_4");
    files.push_back("/tmp/work/feiteng/data/wc/wc_5");
    files.push_back("/tmp/work/feiteng/data/wc/wc_6");
    files.push_back("/tmp/work/feiteng/data/wc/wc_7");
   
    for (std::string& file : files) {
        out_file.open(file.c_str());
        for (int i = 0; i < 500; i++) {
            std::string str = random_string(rand() % 10) + " ";
            out_file << str;
        }
        out_file.close();
    }
    
}

int main(int argc, char **argv)
{
    gen_words();
    //"word count + uuid is the job name"
	if (argc != 3) {
		std::cout << "Usage: ./wc num_of_mappers num_of_reducers" << std::endl;
		exit(-1);
	}
    twister::job_config* job_conf = twister::job_config::getInstance("word_count_" + boost::lexical_cast<std::string>(boost::uuids::random_generator()()));
    
    std::cout << "mr job " << job_conf->job_name << " starts now." << std::endl;
    //job_conf->udf_bin_path = "/Users/feiteng/Library/Developer/Xcode/DerivedData/Twister-cpp-bhxrgrfnxsvvhwfeqbijayxvhjpd/Build/Products/Debug/";
	const char* twister_home = getenv("TWISTER_HOME");
	if (twister_home == NULL) {
		std::cerr << "please set TWISTER_HOME env properly" << std::endl;
		exit(-1);
	}
	std::string sample_lib_path(twister_home);
	sample_lib_path += "/Twister-cpp/samples/lib/";
	job_conf->udf_bin_path = sample_lib_path;
    job_conf->mapper_name = "word_count_mapper";
    job_conf->reducer_name = "word_count_reducer";
    job_conf->gather_name = "word_count_gather";
    job_conf->udf_dy_lib = "libwordcount.so";
    job_conf->num_mappers = atoi(argv[1]);
    job_conf->num_reducers = atoi(argv[2]);
    
    twister::driver td(job_conf);
    
    //"wc" here is the prefix of partition files
    td.runMapReduceJob(new twister::string_value("wc"));
    
    std::map<std::string, int> final_res = static_cast<word_count_gather*>(td.get_gather())->get_result();
    std::map<std::string, int>::iterator it = final_res.begin();
    for (; it != final_res.end(); it++) {
        if (it->first == "\n") std::cout << "\\n" << " : " << it->second << std::endl;
        else if (it->first == "\t") std::cout << "\\t" << " : " << it->second << std::endl;
        else if (it->first == "\b")  std::cout << "\\b" << " : " << it->second << std::endl;
        else  std::cout << it->first << " : " << it->second << std::endl;
    }
    
    return 0;
}

