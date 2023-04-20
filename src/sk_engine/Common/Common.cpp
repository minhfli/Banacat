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
    std::ifstream file(path);

    if (!file) FatalError("Cannot open file: " + path);

    std::string return_string((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());

    return return_string;
}

nlohmann::json ReadJsonFile(const std::string path) {
    nlohmann::json data = nlohmann::json::parse(ReadTextFile(path));
    if (data.is_discarded()) std::cout << "json parse error";

    return data;
}

#include <cstdlib>
#include <ctime>

float RandomFloat(const float a, const float b) {
    return a + (b - a) * (float(rand()) / float(RAND_MAX));
}
