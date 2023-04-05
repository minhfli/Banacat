#pragma once

#define GLM_FORCE_SWIZZLE
#include <GLM/glm.hpp>

namespace  sk_physic2d {

    struct rect {

        /// @brief center pos
        glm::vec2 pos = glm::vec2(0);
        /// @brief half size
        glm::vec2 hsize = glm::vec2(1);

        rect(glm::vec2 p = glm::vec2(0), glm::vec2 hs = glm::vec2(1)): pos(p), hsize(hs) {}
        rect(glm::vec4 bound) {
            pos = (glm::vec2(bound.z, bound.w) + glm::vec2(bound.x, bound.y)) / 2.0f;
            hsize = (glm::vec2(bound.z, bound.w) - glm::vec2(bound.x, bound.y)) / 2.0f;
        }

        inline rect expand(float x, float y) {
            return rect(pos, hsize + glm::vec2(x, y));
        }
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
                this->pos.x > r2.pos.x - r2.hsize.x - this->hsize.x &&
                this->pos.y > r2.pos.y - r2.hsize.y - this->hsize.y &&
                this->pos.x < r2.pos.x + r2.hsize.x + this->hsize.x &&
                this->pos.y < r2.pos.y + r2.hsize.y + this->hsize.y
            };
        }
    };

    struct irect {
        const int precision = 32;
        glm::ivec2 pos;
        glm::ivec2 hsize;
        glm::vec2 offset = glm::vec2(0);
        irect(glm::ivec2 p = glm::ivec2(0), glm::ivec2 hs = glm::ivec2(1)): pos(p), hsize(hs) {}

        inline irect expand(int x, int y) {
            hsize.x += x;
            hsize.y += y;
        }
        inline glm::ivec4 bound(glm::ivec2 expand = glm::ivec2(0)) const {
            return glm::ivec4(pos - hsize - expand, pos + hsize + expand);
        }
        inline bool contain(const irect& r2) const {
            return {
                r2.pos.x - r2.hsize.x >= this->pos.x - this->hsize.x &&
                r2.pos.y - r2.hsize.y >= this->pos.y - this->hsize.y &&
                r2.pos.x + r2.hsize.x <= this->pos.x + this->hsize.x &&
                r2.pos.y + r2.hsize.y <= this->pos.y + this->hsize.y
            };
        }
        inline bool overlap(const irect& r2) const {
            return{
                this->pos.x > r2.pos.x - r2.hsize.x - this->hsize.x &&
                this->pos.y > r2.pos.y - r2.hsize.y - this->hsize.y &&
                this->pos.x < r2.pos.x + r2.hsize.x + this->hsize.x &&
                this->pos.y < r2.pos.y + r2.hsize.y + this->hsize.y
            };
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
        int type;
        rect RECT;
        uint32_t tag;

        /// @param t collider type, 0:solid, 1:actor, 2:triggerer
        /// @param tg tag
        Body_Def(rect r, int t = 0, uint32_t tg = 0):
            type(t),
            RECT(r),
            tag(tg) {}
    };
    struct Body {
        public:
        bool is_active = false;
        uint8_t type;
        uint32_t tag;

        /// @brief this should be set to true for the physic world to know and update 
        bool modified = false;

        rect RECT;
        glm::vec2 velocity = glm::vec2(0);

        bool is_solid() const { return is_active && type == 0; }
        bool is_actor() const { return is_active && type == 1; }

        Body() {}
        Body(Body_Def def):
            is_active(true),
            modified(false),
            type(def.type),
            RECT(def.RECT),
            tag(def.tag) {}
    };

}
