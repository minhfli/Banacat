#include "AABB.h"

namespace sk_physic2d {
    bool point_in_rect(const glm::vec2& p, const rect& r) {
        return (
            p.x >= r.pos.x &&
            p.y >= r.pos.y &&
            p.x <= r.pos.x + r.size.x &&
            p.y <= r.pos.y + r.size.y
        );
    }

    bool rect_in_rect(const rect& r1, const rect& r2) {
        return (
            r1.pos.x >= r2.pos.x &&
            r1.pos.y >= r2.pos.y &&
            r1.pos.x + r1.size.x <= r2.pos.x + r2.size.x &&
            r1.pos.y + r1.size.y <= r2.pos.y + r2.size.y
        );
    }

    bool rect_vs_rect(const rect& r1, const rect& r2) {
        return (
            r1.pos.x + r1.size.x >= r2.pos.x &&
            r2.pos.x + r2.size.x >= r1.pos.x &&
            r1.pos.x + r1.size.y >= r2.pos.y &&
            r2.pos.x + r2.size.y >= r1.pos.y
        );
    }
    bool ray_vs_rect(const ray& _ray, const rect& r) {

    }

}