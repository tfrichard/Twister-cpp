//
//  in_archive.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/1/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__in_archive__
#define __Twister_cpp__in_archive__

#include <iostream>
//#include <Twister-cpp/message/message.h>


namespace twister {
    class message;
    
    class in_archive{
    public:
        std::istream* istrm;
        char* buf;
        std::size_t off;
        std::size_t len;
        
        in_archive(std::istream &istrm_) : istrm(&istrm_), buf(NULL), off(0), len(0) {}
        in_archive(char* buf_, std::size_t len_) : istrm(NULL), buf(buf_), off(0), len(len_){}
        
        char read_char() {
            if (buf) {
                return buf[off++];
            } else {
                char c;
                istrm->get(c);
                return c;
            }
        }

        void read(char *c, int l) {
            if (buf) {
                memcpy(c, buf + off, l);
                off += l;
            } else {
                istrm->read(c, l);
            }
        }
        
        void reset() {
            off = 0;
        }
    };

    in_archive& operator>>(in_archive& in_arc, message& msg);
    in_archive& operator>>(in_archive& in_arc, int&);
    in_archive& operator>>(in_archive& in_arc, std::string&);
}

#endif /* defined(__Twister_cpp__in_archive__) */
