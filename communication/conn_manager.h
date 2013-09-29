//
//  conn_manager.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/21/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__conn_manager__
#define __Twister_cpp__conn_manager__

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <Twister-cpp/worker/daemon.h>
#include <Twister-cpp/pub/constants.h>

namespace twister {
    class conn_manager {
    public:
        conn_manager();
        virtual ~conn_manager();
        bool spawn_daemons_from_env();
        
    public:
        //std::vector<daemon*> daemon_ptrs;
        std::vector<std::string> ip_list;
        int num_of_managed_daemon;
    
    private:
        inline bool is_spawn_from_env() {
            if (getenv("MPI") == NULL) return true;
            else return false;
        }
    };
}

#endif /* defined(__Twister_cpp__conn_manager__) */
