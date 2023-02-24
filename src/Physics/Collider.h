#pragma once
#include <GLM/glm.hpp>

namespace  sk_physic2d {
    struct rect {
        bool fixed = 0;                    //* is static ?
        glm::vec2 pos = glm::vec2(0);      //* bottom left point of the rect
        glm::vec2 size = glm::vec2(1);
        glm::vec2 velocity = glm::vec2(0);

        rect() {}
        rect(glm::vec2 p = glm::vec2(0), glm::vec2 s = glm::vec2(1), glm::vec2 v = glm::vec2(0)) {
            pos = p;
            size = s;
            velocity = v;
        }
        inline glm::vec2 center() const {
            return glm::vec2(pos + size / 2.0f);
        }
    };
    struct circle {
        glm::vec2 pos = glm::vec2(0);
        float radius = 1.0f;
        glm::vec2 velocity = glm::vec2(0);

        circle() {};
        circle(glm::vec2 p = glm::vec2(0), float r = 0) {
            pos = p;
            radius = r;
        }
        inline rect bounding_box() const {
            return rect(glm::vec2(pos.x - radius, pos.y - radius), glm::vec2(radius * 2), velocity);
        }
    };

}
