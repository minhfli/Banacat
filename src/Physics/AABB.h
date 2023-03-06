#pragma once
#include <iostream>

#include <GLM/glm.hpp>
#include <cmath>
#include <algorithm>
#include "Collider.h"

namespace sk_physic2d {
    //* simple check 
    bool point_in_rect(const glm::vec2& p, const rect& r);
    bool point_in_rect_bound(const glm::vec2& p, const rect& r);
    bool point_in_rect_bound(const glm::vec2& p, const rect& r, glm::vec2& normal);
    bool rect_in_rect(const rect& r1, const rect& r2);
    bool rect_vs_rect(const rect& r1, const rect& r2);
    bool ray_vs_rect(const ray& l, const rect& r, contact* contact_data = nullptr);
    bool ray_vs_ray(const ray& l1, const ray& l2, contact* contact_data = nullptr);

    //* collision resolve
    /// @brief resolve 2 rect that collided
    /// @param r1 
    /// @param r2 
    void resolve_colli_rect(const rect& r1, const rect& r2);
    /// @brief resolve 2 rect that may collide
    /// @param r1 
    /// @param r2 must be static
    void resolve_swept_rect(const rect& r1, const rect& r2);
}