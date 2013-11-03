//
//  in_archive.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 10/1/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/serialization/in_archive.h>
#include <Twister-cpp/message/message.h>

namespace twister {
    in_archive& operator>>(in_archive& in_arc, message& msg) {
        msg.load(in_arc);
        return in_arc;
    }
    
    in_archive& operator>>(in_archive& in_arc, int& i) {
        in_arc.read(reinterpret_cast<char*>(&i), sizeof(int));
        return in_arc;
    }
    
    in_archive& operator>>(in_archive& in_arc, std::string& str) {
        int len;
        in_arc >> (int&)len;
        
        str.resize(len);
        in_arc.read(const_cast<char*>(str.c_str()), (int)len);
        return in_arc;
    }
}
