#pragma once

#include <GLM/glm.hpp>
#include "Collider.h"

namespace sk_physic2d {
    //* simple check 
    bool point_in_rect(const glm::vec2& p, const rect& r);
    bool rect_in_rect(const rect& r1, const rect& r2);
    bool rect_vs_rect(const rect& r1, const rect& r2);
    bool ray_vs_rect(const ray& l, const rect& r);
    bool ray_vs_ray(const ray& l1, const ray& l2);

    //* collision respone
    rect res_rr(const rect& r1, const rect& r2);
}