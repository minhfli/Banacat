#pragma once

#include <GLM/glm.hpp>
#include "Collider.h"

namespace sk_physic2d {
    //* simple check 
    bool point_in_rect(const glm::vec2& p, const rect& r);
    bool rect_in_rect(const rect& r1, const rect& r2);
    bool rect_vs_rect(const rect& r1, const rect& r2);
    bool ray_vs_rect();

}