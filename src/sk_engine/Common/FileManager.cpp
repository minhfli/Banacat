#include "FileManager.h"

#include <unordered_map>

namespace { //private data
    std::unordered_map<std::string, unsigned int> string_int_map;
    std::unordered_map<std::string, void*> string_ptr_map;
}

namespace File_Manager {
    bool find_string_i(const std::string& str, unsigned int* id) {
        auto find = string_int_map.find(str);
        if (find != string_int_map.end()) {
            *id = find->second;
            return true;
        }
        return false;
    }

    bool find_string_p(const std::string& str, void** ptr) {
        auto find = string_ptr_map.find(str);
        if (find != string_ptr_map.end()) {
            *ptr = find->second;
            return true;
        }
        return false;
    }

    void add_string_i(const std::string& str, unsigned int id) {
        string_int_map.insert({ str,id });
    }

    void add_string_p(const std::string& str, void* ptr) {
        string_ptr_map.insert({ str,ptr });
    }
}