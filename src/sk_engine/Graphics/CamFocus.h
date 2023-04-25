#pragma once

#include <GLM/glm.hpp>

namespace sk_graphic {
    enum class CamMode {
        NONE,

        UPDATE_FIXED,
        UPDATE_SMOOTH,

        TRANSITION_SINE,
        TRANSITION_LINEAR,
    };
    class CamFocus {
        public:
        glm::vec2 pos;
        glm::vec2 offset = glm::vec2(0);

        // how fast the camera will move
        glm::vec2 damping = glm::vec2(3, 5);

        // if the camera is outside this zone, it will be clamped to this zone
        glm::vec2 softzone = glm::vec2(4, 3);

        glm::vec4 cambound = glm::vec4(-1000, -1000, 1000, 1000);
        glm::vec2 cam_hsize = glm::vec2(20, 11.5f);

        CamMode update_mode = CamMode::UPDATE_SMOOTH;

        CamMode transition_mode = CamMode::TRANSITION_SINE;

        bool in_transition = false;
        glm::vec2 transition_start_pos;
        glm::vec2 transition_end_pos;
        float transition_time;
        float transition_start_time;

        glm::vec2 GetTarget_pos(glm::vec2 cam_pos);
        glm::vec2 GetTransition_pos();

        bool is_transitioning();
        void SetTransition(glm::vec2 start_pos, glm::vec2 end_pos, float time, CamMode mode = CamMode::TRANSITION_SINE);
    };
}