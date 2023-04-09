#pragma once

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

    const int INTCOORD_PRECISION = 16;
    const float INTCOORD_ONE_OVER_PRECISION = 1.0f / INTCOORD_PRECISION;
    /// @brief physic rectangle collider in integer coodinate
    struct irect {
        glm::ivec4 bound;
        /// @brief offet of the rect to its true positon in integer coodinate, always smaller than 1, should not be set outside class
        glm::vec2 offset = glm::vec2(0);

        // simple constuctors
        irect():bound(0) {}
        irect(const int x, const int y, const int z, const int w): bound(x, y, z, w) {}
        irect(const glm::ivec4 b): bound(b) {}

        /// @brief advanced contructor, world size, world position   
        static irect irect_fsize_fpos(glm::vec2 hsize, glm::vec2 pos);
        /// @brief advanced contructor, world bound   
        static irect irect_fbound(glm::vec4 bound);

        /// @brief center of rect in integer coordinate, can return false result if rect size is not even
        glm::ivec2 centeri()const;
        /// @brief true center of rect in world coordinate
        glm::vec2 true_center()const;
        /// @brief true bound of rect in world coordinate
        glm::vec4 true_bound(bool add_offset = 1) const;

        /// @brief move rect in integer coordinate
        /// @param offset_0 if true, set offset to 0
        void movei(const int x, const int y, const bool offset_0 = 0);
        /// @brief move rect in world(float) coordinate
        void move(const float x, const float y);
        /// @brief set center of the rect in world coordinate
        void set_center(float x, float y);

        void expand(int x, int y);
        void extend(int x, int y);
        /// @brief return new expanded rect 
        irect expand_(int x, int y);
        /// @brief return new extended rect 
        irect extend_(int x, int y);

        bool contain(const irect& r2) const;
        bool overlap(const irect& r2) const;
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
        irect RECT;
        uint32_t tag;

        /// @param t collider type, 0:solid, 1:actor, 2:triggerer
        /// @param tg tag
        Body_Def(irect r, int t = 0, uint32_t tg = 0):
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

        irect RECT;
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
