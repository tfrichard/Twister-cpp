//
//  out_archive.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 10/1/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/serialization/out_archive.h>
#include <Twister-cpp/message/message.h>

namespace twister {
    out_archive& operator<<(out_archive& out_arc, int i) {
        out_arc.direct_assign(i);
        return out_arc;
    }
    
    out_archive& operator<<(out_archive& out_arc, const message& msg) {
        msg.save(out_arc);
        return out_arc;
    }
    
    out_archive& operator<<(out_archive& out_arc, const std::string& str) {
        int len = str.length();
        out_arc << (int)len;
        out_arc.write(reinterpret_cast<const char*>(str.c_str()), len);
        return out_arc;
    }
}
