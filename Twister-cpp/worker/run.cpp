//
//  run.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/worker/daemon.h>
#include <Twister-cpp/worker/mapper.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/string_key.h>
#include <Twister-cpp/types/string_value.h>
#include <thread>
#include <boost/asio.hpp>
#include <dlfcn.h>
#include <unistd.h>

using namespace twister;

/*
 *IN: daemon_id, daemon_ip, driver_ip
 */

int main(int argc, char* argv[]) {
//    void* dl_handle = dlopen("libapp_test_dl.dylib", RTLD_NOW);
//    
//    if (dl_handle == NULL) {
//        std::cerr << dlerror() << std::endl;
//        exit(-1);
//    }
//    
//    //typedef mapper* (mapper_t_ptr)();
//    mapper_t_ptr* mapper_hook = (mapper_t_ptr*)dlsym(dl_handle, "create_instance");
//    if (dlerror()) {
//        std::cerr << dlerror() << std::endl;
//        exit(-1);
//    }
//    mapper* real_mapper = mapper_hook();
//    real_mapper->map(new map_output_collector, string_key("key"), string_value("value"));
//    dlclose(dl_handle);
   	if (argc != 4) {
		std::cerr << "Usage: ./twister_daemon daemon_id daemon_ip driver_ip" << std::endl;
		exit(-1);
	} 

    using twister::daemon;
    using boost::asio::ip::tcp;
	int daemon_id = atoi(argv[1]);
	std::string daemon_ip((const char*)argv[2]);
	std::string driver_ip((const char*)argv[3]);

    boost::asio::io_service io_service;
    tcp::endpoint end_point(tcp::v4(), 12500 + daemon_id);
    
    daemon d(io_service, end_point, daemon_id, daemon_ip, driver_ip);
    
    io_service.run();
    
    //std::thread t1(&daemon::foo, d);
    //t1.join();
    return 0;
}


