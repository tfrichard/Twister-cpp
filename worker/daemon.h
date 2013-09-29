//
//  daemon.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__daemon__
#define __Twister_cpp__daemon__

#include <iostream>
#include <string>
#include <boost/asio.hpp>

namespace twister {
    using boost::asio::ip::tcp;
    
    class daemon {
    private:
        std::string ip_addr;
        short port;
        int daemon_id;
        
        tcp::acceptor acceptor;
        tcp::socket socket;
       
    public:
        daemon(boost::asio::io_service&, const tcp::endpoint &);
        virtual ~daemon();
        void foo();
        
    private:
        void do_accept();
        
        
    };
}

#endif /* defined(__Twister_cpp__daemon__) */
