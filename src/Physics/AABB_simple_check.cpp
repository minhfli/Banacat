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

    bool ray_vs_rect(const ray& l, const rect& r) {
        float xmin = r.pos.x;
        float ymin = r.pos.y;
        float xmax = r.pos.x + r.size.x;
        float ymax = r.pos.y + r.size.y;


        bool oky = 0, okx = 0;

        if (l.dir.y == 0) oky = 1;
        else {
            float tL = (ymin - l.pos.y) / l.dir.y;
            if (tL >= 0 && tL <= 1) oky = 1;
            float tR = (ymax - l.pos.y) / l.dir.y;
            if (tR >= 0 && tR <= 1) oky = 1;
        }

        return (oky && okx);
    }

}