#pragma once

#define GLM_FORCE_SWIZZLE
#include <GLM/glm.hpp>

namespace  sk_physic2d {

    struct rect {

        /// @brief center pos
        glm::vec2 pos = glm::vec2(0);
        /// @brief half size
        glm::vec2 hsize = glm::vec2(1);

        rect(glm::vec2 p = glm::vec2(0), glm::vec2 s = glm::vec2(1)): pos(p), hsize(s / 2.0f) {}
        rect(glm::vec4 bound) {
            pos = (glm::vec2(bound.z, bound.w) + glm::vec2(bound.x, bound.y)) / 2.0f;
            hsize = (glm::vec2(bound.z, bound.w) - glm::vec2(bound.x, bound.y)) / 2.0f;
        }

        /// @brief lower left and upper right conner position of the rect
        /// @return 
        inline glm::vec4 bound(glm::vec2 expand = glm::vec2(0)) const {
            return glm::vec4(pos - hsize - expand, pos + hsize + expand);
        }
        inline bool contain(const rect& r2) const {
            return {
                r2.pos.x - r2.hsize.x >= this->pos.x - this->hsize.x &&
                r2.pos.y - r2.hsize.y >= this->pos.y - this->hsize.y &&
                r2.pos.x + r2.hsize.x <= this->pos.x + this->hsize.x &&
                r2.pos.y + r2.hsize.y <= this->pos.y + this->hsize.y
            };
        }
        inline bool overlap(const rect& r2) const {
            return{
                this->pos.x >= r2.pos.x - r2.hsize.x - this->hsize.x &&
                this->pos.y >= r2.pos.y - r2.hsize.y - this->hsize.y &&
                this->pos.x <= r2.pos.x + r2.hsize.x + this->hsize.x &&
                this->pos.y <= r2.pos.y + r2.hsize.y + this->hsize.y
            };
        }
    };
    struct circle {
        glm::vec2 pos = glm::vec2(0);
        float radius = 1.0f;

        circle() {};
        circle(glm::vec2 p = glm::vec2(0), float r = 0) {
            pos = p;
            radius = r;
        }
        inline rect bound() const {
            return rect(glm::vec2(pos.x - radius, pos.y - radius), glm::vec2(radius * 2));
        }
    };
    struct ray {
        glm::vec2 pos = glm::vec2(0);
        glm::vec2 dir = glm::vec2(0, 1); // default is up

        ray() {};
        ray(const glm::vec2 p = glm::vec2(0), const glm::vec2 d = glm::vec2(0, 1)): pos(p), dir(d) {}

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

    //! may change in future implementation 
    struct Body_Def {
        bool solid;
        rect RECT;
        glm::vec2 start_velocity;

        Body_Def(rect r, bool s = true, glm::vec2 v = glm::vec2(0)):solid(s), RECT(r), start_velocity(v) {}
    };
    struct Body {
        friend class AABB_World;
        private:
        bool is_active = false;

        bool modified = false;
        bool is_solid = true;

        rect RECT;
        glm::vec2 velocity;

        public:
        Body() {}
        Body(Body_Def def):
            is_active(true),
            modified(false),
            is_solid(def.solid),
            RECT(def.RECT),
            velocity(def.start_velocity) {}

        /// @brief set the position of the body
        inline void set_pos(const glm::vec2 p) {
            RECT.pos = p;
            modified = true;
        }
        inline void set_size(const glm::vec2 s) {
            RECT.hsize = s / 2.0f;
            modified = true;
        }
    };

}
