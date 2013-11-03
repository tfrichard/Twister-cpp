//
//  key.h
//  Twister-cpp
//
//  Created by Fei Teng on 9/20/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__key__
#define __Twister_cpp__key__

#include <iostream>
#include <Twister-cpp/message/message.h>
#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/serialization/out_archive.h>

namespace twister {
    class key : public message {
    public:
        key() {}
        virtual ~key() {};
        
        virtual void load(in_archive& in_arc) {}
        
        virtual void save(out_archive& out_arc) const {}
        
        virtual bool operator <(const key&) const { return true; };
        
        virtual std::string get_class_name() const { return "key"; }
        
        virtual int hash_code() const { return 0; }
    };
    
    typedef key* key_t_ptr();
}

#endif /* defined(__Twister_cpp__key__) */
