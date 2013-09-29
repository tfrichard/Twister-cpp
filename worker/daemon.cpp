//
//  daemon.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/worker/daemon.h>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace twister {
    using boost::asio::ip::tcp;
    
    //daemon::daemon() {}
    
    daemon::daemon(boost::asio::io_service &io_service_, const tcp::endpoint &end_point_) : \
    acceptor(io_service_, end_point_), socket(io_service_)
    {
        do_accept();
        port = end_point_.port();
    }
    
    void daemon::do_accept() {
        //start up the daemon server
        std::string msg("I'm daemon server!");
        
        try {
            for (;;) {
                std::cout << "daemon is listening..." << std::endl;
                acceptor.accept(socket);
                std::cout << "connection accepted" << std::endl;
                boost::system::error_code ignored_error;
                boost::asio::write(socket, boost::asio::buffer(msg), ignored_error);
            }
            
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

    }
    
    daemon::~daemon() {
        
    }
    
    void daemon::foo() {
        std::cout << "here's foo in daemon!" << std::endl;
    }
}
