//
//  task_status.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/3/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__task_status__
#define __Twister_cpp__task_status__

#include <iostream>
#include <Twister-cpp/message/message.h>

namespace twister {
    enum TASK_TYPE {
        MAP_TASK = 0, REDUCE_TASK = 1, COMBINER_TASK = 2
    };
    enum TASK_EXEC_STATUS {SUCCESS = 0, FAIL = 1};
    
    //message twister tasks use to communicate with twister synchronizer
    class task_status_msg : public message
    {
    public:
        unsigned int task_no;
        TASK_TYPE task_type;
        unsigned int daemon_no;
        unsigned int iteration;
        TASK_EXEC_STATUS task_exec_status;
        
    };
}

#endif /* defined(__Twister_cpp__task_status__) */
