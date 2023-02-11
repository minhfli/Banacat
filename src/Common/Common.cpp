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
std::string ReadFile(const char* filepath) {
    std::string return_string;
    std::ifstream file(filepath);

    //Source file loaded
    if (file)
        return_string.assign((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());
    else FatalError("Cannot open file: " + (std::string)filepath);

    return return_string;
}

