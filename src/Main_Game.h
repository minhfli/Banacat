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

    void Awake();

    void Start();

    void Update(const int delta_tick);

    void Draw();

    //void LateUpdate();

    //void FixedUpdate();

    void Quit();

    inline void out() { std::cout << "b\a\n"; }
}
