#include "Collider.h"

#include <algorithm>

namespace  sk_physic2d {

    irect irect::irect_fsize_fpos(glm::vec2 hsize, glm::vec2 pos) {
        return irect_fbound({ pos - hsize,pos + hsize });
    }
    irect irect::irect_fbound(glm::vec4 bound) {
        return irect(glm::ivec4(bound * (float)INTCOORD_PRECISION));
    }

    glm::ivec2 irect::centeri() const {
        return (glm::ivec2(bound.x, bound.y) + glm::ivec2(bound.z, bound.w)) / 2;
    }
    glm::vec2 irect::true_center()const {
        return (glm::vec2(bound.x + bound.z, bound.y + bound.w) * 0.5f + offset) * INTCOORD_ONE_OVER_PRECISION;
    }
    glm::vec4 irect::true_bound(bool add_offset) const {
        if (add_offset)
            return (glm::vec4(bound) + glm::vec4(offset, offset)) * INTCOORD_ONE_OVER_PRECISION;
        return  glm::vec4(bound) * INTCOORD_ONE_OVER_PRECISION;
    }

    void irect::movei(const int x, const int y, const bool offset_0) {
        bound.x += x;
        bound.y += y;
        bound.z += x;
        bound.w += y;
        if (offset_0) offset = glm::vec2(0);
    }
    void irect::move(const float x, const float y) {
        offset += glm::vec2(x, y) * (float)INTCOORD_PRECISION;
        int _x = round(offset.x);
        int _y = round(offset.y);

        bound.x += _x;
        bound.y += _y;
        bound.z += _x;
        bound.w += _y;
    }
    void irect::set_center(float x, float y) {
        x *= INTCOORD_PRECISION; y *= INTCOORD_PRECISION;
        glm::vec2 center = glm::vec2(bound.x + bound.z, bound.y + bound.w) * 0.5f + offset;
        move(x - center.x, y = center.y);
    }

    void irect::expand(int x, int y) {
        bound.x -= x;
        bound.y -= y;
        bound.z += x;
        bound.w += y;
    }
    void irect::extend(int x, int y) {
        bound.x = std::min(bound.x, bound.x + x);
        bound.y = std::min(bound.y, bound.y + y);
        bound.z = std::max(bound.z, bound.z + x);
        bound.w = std::max(bound.w, bound.w + y);
    }
    irect irect::expand_(int x, int y) {
        return irect(bound.x - x, bound.y - y, bound.z + x, bound.w + y);
    }
    irect irect::extend_(int x, int y) {
        return irect(
            std::min(bound.x, bound.x + x),
            std::min(bound.y, bound.y + y),
            std::max(bound.z, bound.z + x),
            std::max(bound.w, bound.w + y)
        );
    }


    bool irect::contain(const irect& r2) const {
        return {
            r2.bound.x >= this->bound.x &&
            r2.bound.z <= this->bound.z &&

            r2.bound.y >= this->bound.y &&
            r2.bound.w <= this->bound.w
        };
    }
    bool irect::overlap(const irect& r2) const {
        return{
            this->bound.z > r2.bound.x &&
            r2.bound.z > this->bound.x &&
            this->bound.w > r2.bound.y &&
            r2.bound.w > this->bound.y
        };
    }
}