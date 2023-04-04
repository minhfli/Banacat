#pragma once
#include <iostream>

#define GLM_FORCE_SWIZZLE
#include <GLM/glm.hpp>
#include <cmath>
#include <algorithm>
#include "Collider.h"

// functions for sk physic 

namespace sk_physic2d {
    //* simple check 
    bool point_in_rect(const glm::vec2& p, const glm::vec4& r, contact* contact_data = nullptr);

    bool rect_contain(const glm::vec4& r1, const glm::vec4& r2);
    bool rect_overlap(const glm::vec4& r1, const glm::vec4& r2);

    bool ray_vs_rect(const ray& l, const glm::vec4& r, contact* contact_data);

    bool sweep_rect_vs_rect(const rect& r1, const rect& r2, glm::vec2 r1offset, contact* contact_data);
    bool colli_rect_vs_rect(const rect& r1, const rect& r2, contact* contact_data);

}