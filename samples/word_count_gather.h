//
//  word_count_gather.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/24/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__word_count_gather__
#define __Twister_cpp__word_count_gather__

#include <iostream>
#include <map>
#include "gather.h"
#include "string_key.h"
#include "int_value.h"
#include "job_config.h"


class word_count_gather : public twister::gather {
private:
    std::map<std::string, int> wc_map;
    
public:
    void do_gather(std::map<const twister::key*, const twister::value*>& key_value_map) {
        std::map<const twister::key*, const twister::value*>::const_iterator it = key_value_map.begin();
        for (; it != key_value_map.end(); it++) {
            const std::string& word = (static_cast<const twister::string_key*>(it->first))->key_str;
            int cnt = (static_cast<const twister::int_value*>(it->second))->value_int;
            
            wc_map[word] = cnt;
            
            std::cout << "final count of word: " << word << " is " << cnt << std::endl;
        }
    }
    
    void configure(const twister::job_config*) {}
    
    void close() {}
    
    const std::map<std::string, int>& get_result() {
        return wc_map;
    }
    
    ~word_count_gather() {}
};


#endif /* defined(__Twister_cpp__word_count_gather__) */
