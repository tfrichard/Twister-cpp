//
//  run.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/23/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/worker/daemon.h>
#include <thread>
#include <boost/asio.hpp>

//int main(int argc, char* argv[]) {
//    using twister::daemon;
//    using boost::asio::ip::tcp;
//    boost::asio::io_service io_service;
//    tcp::endpoint end_point(tcp::v4(), 12500);
//    
//    daemon d(io_service, end_point);
//    
//    io_service.run();
//        
//    //std::thread t1(&daemon::foo, d);
//    //t1.join();
//    return 0;
//}
