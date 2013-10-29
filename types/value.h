//
//  value.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__value__
#define __Twister_cpp__value__

#include <iostream>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

namespace twister {
    class value : public message {
    public:
        value() {}
        virtual ~value() {};
        
        virtual void load(in_archive& in_arc) {}
        
        virtual void save(out_archive& out_arc) const {}
        
        virtual std::string get_class_name() const {
            return "value";
        }
    };
    
    typedef value* value_t_ptr();
}
#endif /* defined(__Twister_cpp__value__) */
