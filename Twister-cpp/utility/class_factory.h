//
//  class_factory.h
//  Twister-cpp
//
//  Created by Fei Teng on 10/16/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#ifndef __Twister_cpp__class_factory__
#define __Twister_cpp__class_factory__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

namespace twister {
    class class_factory {
    private:
        static std::vector<std::string> class_pathes;
        static std::vector<void*> dl_handlers;
        static std::map<std::string, std::string> class_lib_map;
        
        class_factory() {};
        
    public:
        static inline void add_class_path(const std::string& cp) {
            if (find(class_pathes.begin(), class_pathes.end(), cp) == class_pathes.end())
                class_pathes.push_back(cp);
        }
        
        static inline std::string get_dy_func_name(const std::string& cp) { return "create_" + cp + "_instance";}
        static void *get_instance(const std::string class_name);
        static void registe_class(const std::string&, const std::string&);
        static void destory();
        
    };
}

#endif /* defined(__Twister_cpp__class_factory__) */
