#include <iostream>
#include <fstream>

#include "Common.h"

//-----------------------------------------------------------------------------------
void FatalError(std::string string) {
    std::cout << string << '\n';
    system("PAUSE");
    exit(1);
}

void Error(std::string string) {
    std::cout << string << '\n';
}


//-----------------------------------------------------------------------------------
std::string ReadTextFile(const std::string path) {
    std::string return_string;
    std::ifstream file(path);

    //Source file loaded
    if (file)
        return_string.assign((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());
    else FatalError("Cannot open file: " + path);

    return return_string;
}

nlohmann::json ReadJsonFile(const std::string path) {
    std::ifstream file(path);
    if (!file) FatalError("Cannot open file: " + path);

    nlohmann::json data = nlohmann::json::parse(file);
    return data;
}

#include <cstdlib>
#include <ctime>

float RandomFloat(const float a, const float b) {
    return a + (b - a) * (float(rand()) / float(RAND_MAX));
}
