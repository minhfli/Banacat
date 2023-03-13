#pragma once

#include <string>

//Todo    make a file manager

namespace File_Manager {
//? return an id( texture, shader, .. ) that loaded from a file path
    bool find_string_i(const std::string& str, unsigned int* id);

    bool find_string_p(const std::string& str, void** ptr);

    void add_string_i(const std::string& str, unsigned int id);

    void add_string_p(const std::string& str, void* ptr);
}