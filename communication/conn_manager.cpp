//
//  conn_manager.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/communication/conn_manager.h>
#include <boost/foreach.hpp>
#include <cstdlib>

namespace twister {
    conn_manager::conn_manager() {
        
    }
    
    conn_manager::~conn_manager() {
    }
    
    bool conn_manager::spawn_daemons_from_env() {
        if (is_spawn_from_env()) {
            std::string command;
            char *twister_home = getenv("TWISTER_HOME");
            if (twister_home == NULL) {
                std::cerr << "please set TWISTER_HOME properly" << std::endl;
                return false;
            }
            char *hosts_file = getenv("HOST_FILE");
            if (hosts_file == NULL) {
                std::cerr << "HOST_FILE not set. Please startup daemon yourself" << std::endl;
                return false;
            }
            command = std::string(twister_home) + "/bin/spawn_daemon.py";
            std::system(command.c_str());
            return true;
        }
        else return false;
    }
                
}