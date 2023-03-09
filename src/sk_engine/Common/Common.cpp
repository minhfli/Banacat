#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

#include "Error.h"

void FatalError(std::string string) {
    std::cout << string << '\n';
    SDL_Quit();
    system("PAUSE");
    exit(0);
}


#include "ReadFile.h"
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

