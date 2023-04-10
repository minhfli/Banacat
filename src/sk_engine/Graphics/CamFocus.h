#pragma once

#include <GLM/glm.hpp>

namespace sk_graphic {
    enum class CamMode {
        NONE,

        UPDATE_FIXED,
        UPDATE_SMOOTH,

    };
    class CamFocus {
        public:
        glm::vec2 pos;
        glm::vec2 offset = glm::vec2(0);

        // how fast the camera will move
        glm::vec2 damping = glm::vec2(3, 5);

        // if the camera is outside this zone, it will be clamped to this zone
        glm::vec2 softzone = glm::vec2(4, 3);

        bool have_bound = false;
        glm::vec4 cambound;

        CamMode update_mode = CamMode::UPDATE_SMOOTH;
        //CamMode start_mode = CamMode::START_FIXED;
    };
}