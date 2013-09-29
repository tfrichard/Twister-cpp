//
//  driver.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/18/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/client/driver.h>
#include <Twister-cpp/types/value.h>
#include <Twister-cpp/types/key.h>
#include <Twister-cpp/communication/conn_manager.h>

#include <boost/foreach.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <vector>
#include <string>
#include <utility>

namespace twister{
    //log4cxx::LoggerPtr driver::log = log::get_logger("driver");
    
    driver::driver() {
        this->jobconf = job_config::getInstance();
        //test connect with all daemons
        std::pair<unsigned int, std::string> item;
        std::vector<std::string> daemon_ips;
        BOOST_FOREACH(item, jobconf->daemon_map) {
            daemon_ips.push_back(item.second);
        }
        
        BOOST_FOREACH(std::string ip, daemon_ips) std::cout << ip << std::endl;
        //sleep(100);
        
#ifdef _DEBUG
        std::cout << "about to start all daemons by default env" << std::endl;
        conn_mgr = new conn_manager();
        conn_mgr->spawn_daemons_from_env();
        std::cout << "about to test connection" << std::endl;
        test_conn(daemon_ips);
        sleep(10);
#endif
        
    }
    
    driver::~driver() {}
    
    void driver::configMapTasks() {
        int num_of_mappers = jobconf->num_mappers;
        int num_of_daemons = conn_mgr->num_of_managed_daemon;
        
        
    }
    
    void driver::test_conn(std::vector<std::string> ips) {
        try {
            boost::asio::io_service io_service;
            using boost::asio::ip::tcp;
            
            tcp::resolver resolver(io_service);
            
            BOOST_FOREACH(std::string ip, ips) {
                //std::cout << ip << std::endl;
                tcp::resolver::query query(ip, "12500");
                tcp::resolver::iterator endpointer = resolver.resolve(query);
                tcp::socket socket(io_service);
                boost::asio::connect(socket, endpointer);
                //boost::array<char, 128> buf;
                char buf[128];
                memset(buf, 0, 128);
                boost::system::error_code error;
                std::cout << "try to get data from server" << std::endl;
                size_t len = socket.read_some(boost::asio::buffer(buf), error);
                
                if (error == boost::asio::error::eof) exit(1);
                std::cout << std::string(buf) << " " << len << std::endl;
                //std::cout.write(buf.data(), len);
                
                //sleep(100);
            }

        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    
    bool driver::runMapReduceJob(std::vector<std::pair<key, value> > input_pairs) {

        return true;
    };
    
}
