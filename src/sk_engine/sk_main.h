#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <string>

#include <Common/Error.h>
#include <Common/ReadFile.h>
//adad
namespace sk_main {
    void Init();

    void Run();

    void Quit();

    void GameLoop();

    //! Temporary, remove in future

    void Draw();
    void Update();
}
