#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <string>

#include <Common/Error.h>
#include <Common/ReadFile.h>

namespace sk_engine {
    void Init();

    void Run();

    void Awake();

    void Start();

    void Update();

    void Draw();

    //void LateUpdate();

    //void FixedUpdate();

    void Quit();
}
