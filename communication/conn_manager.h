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
#include <map>
#include <set>
#include <cstdlib>
#include <Twister-cpp/worker/daemon.h>
#include <Twister-cpp/pub/constants.h>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/message/ack_msg.h>

namespace twister {
    class conn_manager {
    public:
        conn_manager();
        virtual ~conn_manager();
        bool spawn_daemons_from_env();
        bool bcast_msg(const message&);
        bool send_msg(const message&, int);
        
    public:
        //std::vector<daemon*> daemon_ptrs;
        std::vector<std::string> ip_list;
        int num_of_managed_daemon;
        std::vector<daemon*> active_daemons;
        std::map<int, std::string> daemon_map;
        //<reduce_no, deamon_list>
        std::map<int, std::set<int> > reduce_input_deamon_map;
    
    private:
        inline bool is_spawn_from_env() {
            if (getenv("MPI") == NULL) return true;
            else return false;
        }
        
        void process_ack(in_archive&, const message&);
        
    };
}

#endif /* defined(__Twister_cpp__conn_manager__) */
