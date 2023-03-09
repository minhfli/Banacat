#pragma once

#define SK_KEY_ SK_SCANCODE_

#include <SDL2/SDL.h>
#include <GLM/glm.hpp>

#include "keycode.h"

namespace sk_input {

    void Init();
    void ShutDown();
    void Process();

    /// @brief is key pressed  
    bool Key(const sk_key& key);
    /// @brief is key just pressed 
    bool KeyDown(const sk_key& key);
    /// @brief is key just released
    bool KeyUp(const sk_key& key);

    glm::vec2 MousePos();
}