//
//  message.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/26/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__message__
#define __Twister_cpp__message__

#include <iostream>
#include <map>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

namespace twister {
    //msg types code
    enum MSG_TYPE {INIT_TYPE = -1, START_MAP_TASK = 0, END_MAP_TASK = 1, START_MAPPER = 2, END_MAPPER = 3, START_REDUCE_TASK = 4, END_REDUCE_TASK = 5, START_REDUCER = 6, END_REDUCER = 7, ACK = 8, CONFIG_DAEMON = 9, START_MAP_TASK_GRP = 10, START_REDUCE_TASK_GRP = 11, START_MAPPER_GRP=12,
        START_REDUCER_GRP=13, REDUCE_INPUT=14, REDUCE_INPUT_REQUEST=15, REDUCE_INPUT_MSG=16,  GATHER_INPUT = 17,
        START_GATHER = 18};
    
    enum ACK_TYPE {MAP_TASK_STARTED_ACK = 1, REDUCE_TASK_STARTED_ACK = 2, MAP_START_ACK = 3,
        REDUCE_START_ACK = 4, MAP_TASK_STARTED_GRP_ACK = 5, REDUCE_TASK_STARTED_GRP_ACK = 6,
        MAP_START_GRP_ACK = 7, REDUCE_START_GRP_ACK = 8, REDUCE_INPUT_ACK=9, REDUCE_INPUT_REQUEST_ACK = 10, REDUCE_INPUT_MSG_ACK = 11,
        GATHER_INPUT_ACK = 12, START_GATHER_ACK = 13};
    
    enum {msg_type_len = 1};
    enum {head_length = 4};
    enum {max_body_length = 1024};
        
    static std::map<ACK_TYPE, MSG_TYPE> ack_table = {
        {MAP_TASK_STARTED_ACK, START_MAP_TASK},
        {MAP_START_ACK, START_MAPPER},
        {MAP_TASK_STARTED_GRP_ACK, START_MAP_TASK_GRP},
        {MAP_START_GRP_ACK, START_MAPPER_GRP},
        {REDUCE_TASK_STARTED_ACK, START_REDUCE_TASK},
        {REDUCE_START_ACK, START_REDUCER},
        {REDUCE_TASK_STARTED_GRP_ACK, START_REDUCE_TASK_GRP},
        {REDUCE_INPUT_ACK, REDUCE_INPUT},
        {REDUCE_INPUT_MSG_ACK, REDUCE_INPUT_MSG},
        {REDUCE_INPUT_REQUEST_ACK, REDUCE_INPUT_REQUEST},
        {GATHER_INPUT_ACK, GATHER_INPUT},
        {START_GATHER_ACK, START_GATHER}
    };
    
    class message {
    public:
        MSG_TYPE msg_type;
        
    public:
        message() {
            msg_type = INIT_TYPE;
        }
        
        virtual ~message() {};
        virtual void load(in_archive&) {}
        virtual void save(out_archive&) const {}
    };
  
}

#endif /* defined(__Twister_cpp__message__) */
