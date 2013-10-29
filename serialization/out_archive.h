//
//  out_archive.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/1/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__out_archive__
#define __Twister_cpp__out_archive__

#include <iostream>

namespace twister {
    class message;
    
    class out_archive {
    public:
        std::ostream* ostrm;
        char* buf;
        int off;
        int len;
        
        /// constructor. Takes a generic std::ostream object
        inline out_archive(std::ostream& outstream)
        : ostrm(&outstream),buf(NULL),off(0),len(0) {}
        
        inline out_archive(void)
        : ostrm(NULL),buf(NULL),off(0),len(0) {}
        
        inline void expand_buf(int s) {
            if (off + s > len) {
                len = 2 * (s + len);
                buf = (char*)realloc(buf, len);
            }
        }
        
        /** Directly writes "s" bytes from the memory location
         * pointed to by "c" into the stream.
         */
        inline void write(const char* c, int s) {
            if (ostrm == NULL) {
                expand_buf(s);
                memcpy(buf + off, c, s);
                off += s;
            } else {
                ostrm->write(c, s);
            }
        }
        
        template <typename T>
        inline void direct_assign(const T& t) {
            if (ostrm == NULL) {
                expand_buf(sizeof(T));
                (*reinterpret_cast<T*>(buf + off)) = t;
                off += sizeof(T);
            }
            else {
                T localt = t;
                ostrm->write(reinterpret_cast<char*>(&localt), sizeof(T));
            }
        }
        
        inline void advance(int s) {
            if (ostrm == NULL) {
                expand_buf(s);
                off += s;
            } else {
                ostrm->seekp(s, std::ios_base::cur);
            }
        }
        
        ~out_archive() {}


    };
    
    out_archive& operator<<(out_archive& out_arc, int);
    out_archive& operator<<(out_archive& out_arc, const message&);
    out_archive& operator<<(out_archive& out_arc, const std::string&);
}

#endif /* defined(__Twister_cpp__out_archive__) */
