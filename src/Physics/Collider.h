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
        inline void set_center(const glm::vec2& c) {
            pos = c - size / 2.0f;
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
    struct ray {
        glm::vec2 pos = glm::vec2(0);
        glm::vec2 dir = glm::vec2(0, 1); // default is up

        ray() {};
        ray(const glm::vec2 p = glm::vec2(0), const glm::vec2 d = glm::vec2(0, 1)) {
            pos = p;
            dir = d;
        }

        inline rect bounding_box() const {
            glm::vec2 p = pos;
            glm::vec2 s = dir;
            if (s.x < 0) {
                p.x += s.x;
                s.x = -s.x;
            }
            if (s.y < 0) {
                p.y += s.y;
                s.y = -s.y;
            }
            return rect(p, s);
        }
        inline glm::vec2 begin() const { return pos; }
        inline glm::vec2 end() const { return pos + dir; }

        inline ray reverse() const {
            return ray(pos + dir, -dir);
        }
    };

    struct contact {
        bool hit = false;
        glm::vec2 pos;      //? contact point
        glm::vec2 normal;   //? contact normal
        float t_near = 0;
        float t_far = 0;
        //? time till collide (realtime)
    };
}
