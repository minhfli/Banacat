#include "Json_parse.h"

#include <Common/Error.h>
#include <fstream>
#include <iostream>

void readfile(std::string path) {
    std::ifstream file(path);
    if (!file) FatalError("Cannot open file: " + path);

    nlohmann::json data = nlohmann::json::parse(file);
    std::cout << data["n"][1];
}