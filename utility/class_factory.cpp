//
//  class_factory.cpp
//  Twister-cpp
//
//  Created by Fei Teng on 10/16/13.
//  Copyright (c) 2013 Fei Teng. All rights reserved.
//

#include <Twister-cpp/utility/class_factory.h>
#include <dlfcn.h>

namespace twister {
    
    std::vector<std::string> class_factory::class_pathes = std::vector<std::string>();
    std::vector<void*> class_factory::dl_handlers = std::vector<void*>();
    std::map<std::string, std::string> class_factory::class_lib_map = {
        {"key", "libkey_value_types.dylib"},
        {"value", "libkey_value_types.dylib"},
        {"string_key", "libkey_value_types.dylib"},
        {"string_value", "libkey_value_types.dylib"},
        {"int_value", "libkey_value_types.dylib"},
        {"reduce_input", "libkey_value_types.dylib"},
        //{"word_count_mapper", "libapp_test_dl.dylib"},
        //{"word_count_reducer", "libapp_test_dl.dylib"},
        //{"word_count_gather", "libapp_test_dl.dylib"},
        //{"libapp_test_dl.dylib", "libapp_test_dl.dylib"}
    };
    
    void* class_factory::get_instance(const std::string class_name) {
        std::string dl_path = "";
        void* dl_handle = NULL;
        for (std::string path : class_pathes) {
            std::string absolute_path = path + class_lib_map[class_name];
            //std::cout << "dynamic file path: " << absolute_path << std::endl;
            if ((dl_handle = dlopen(absolute_path.c_str(), RTLD_NOW)) != NULL) {
                break;
            }
        }
        
        if (dl_handle == NULL) {
            std::cerr << "cannot find the class in all pathes! exit" << std::endl;
            exit(-1);
        }
        
        std::string function_name = class_factory::get_dy_func_name(class_name);
        void *res = dlsym(dl_handle, function_name.c_str());
        if (dlerror()) {
            std::cerr << dlerror() << std::endl;
            exit(-1);
        }
        
        return res;
    }
    
    void class_factory::registe_class(const std::string &udf_class, const std::string &udf_dylib) {
        if (class_lib_map.find(udf_class) == class_lib_map.end()) {
            class_lib_map[udf_class] = udf_dylib;
        }
    }
    
    void class_factory::destory() {
        for (void* dl_handle : dl_handlers) {
            dlclose(dl_handle);
        }
    }
}
