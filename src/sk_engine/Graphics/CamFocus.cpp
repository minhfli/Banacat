#include "CamFocus.h"

#include <iostream>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>

#include <Common/sk_time.h>

namespace { // some helper function

    inline float move_to(const float a, const float b, const float step) {
        if (a >= b) {
            if (a - step >= b)
                return a - step;
            return b;
        }
        else {
            if (a + step <= b)
                return a + step;
            return b;
        }
    }

    // a: reference,    b: result
    inline glm::vec2 get_relative_lerp(float a, float a1, float a2, glm::vec2 b1, glm::vec2 b2) {
        return b1 + (a - a1) / (a2 - a1) * (b2 - b1);
    }
    inline  glm::vec2 get_relative_lsin(float a, float a1, float a2, glm::vec2  b1, glm::vec2  b2) {
        return b1 + (float)(sin((a - a1) / (a2 - a1) * 1.57f)) * (b2 - b1);
    }
}
namespace sk_graphic {
    glm::vec2 CamFocus::GetTarget_pos(glm::vec2 cam_pos) {
        if (update_mode == CamMode::UPDATE_FIXED)
            cam_pos = pos + offset;
        if (update_mode == CamMode::UPDATE_SMOOTH) {

            glm::vec2 target = pos + offset;

            float lerp_x = damping.x * sk_time::delta_time;
            float lerp_y = damping.y * sk_time::delta_time;

            cam_pos.x += (target.x - cam_pos.x) * lerp_x;
            cam_pos.y += (target.y - cam_pos.y) * lerp_y;

            //cam_pos.x = std::clamp(cam_pos.x, target.x - softzone.x, target.x + softzone.x);
            //cam_pos.y = std::clamp(cam_pos.y, target.y - softzone.y, target.y + softzone.y);
        }
        cam_pos.x = std::min(cam_pos.x, cambound.z - cam_hsize.x);
        cam_pos.x = std::max(cam_pos.x, cambound.x + cam_hsize.x);
        cam_pos.y = std::min(cam_pos.y, cambound.w - cam_hsize.y);
        cam_pos.y = std::max(cam_pos.y, cambound.y + cam_hsize.y);

        return cam_pos;
    }
    glm::vec2 CamFocus::GetTransition_pos() {
        if (transition_mode == CamMode::TRANSITION_SINE) {
            return get_relative_lsin(
                sk_time::current_time - transition_start_time,
                0,
                transition_time,
                transition_start_pos,
                transition_end_pos
            );
        }
        if (transition_mode == CamMode::TRANSITION_LINEAR) {
            return get_relative_lerp(
                sk_time::current_time - transition_start_time,
                0,
                transition_time,
                transition_start_pos,
                transition_end_pos
            );
        }
        return glm::vec2(0);
    }
    void CamFocus::SetTransition(glm::vec2 start_pos, glm::vec2 end_pos, float time, CamMode mode) {
        in_transition = true;
        transition_start_pos = start_pos;
        transition_end_pos = end_pos;
        transition_time = time;
        transition_mode = mode;

        transition_start_time = sk_time::current_time;
    }
    bool CamFocus::is_transitioning() {
        if (in_transition)
            if (sk_time::current_time - transition_start_time > transition_time) in_transition = false;
        return in_transition;
    }
}