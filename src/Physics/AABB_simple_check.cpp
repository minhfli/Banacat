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
    bool point_in_rect_bound(const glm::vec2& p, const rect& r) {
        if (p.x == r.pos.x || p.x == r.pos.x + r.size.x) return (p.y >= r.pos.y && p.y <= r.pos.y + r.size.y);
        if (p.y == r.pos.y || p.y == r.pos.y + r.size.y) return (p.x >= r.pos.x && p.x <= r.pos.x + r.size.x);
        return 0;
    }
    bool point_in_rect_bound(const glm::vec2& p, const rect& r, glm::vec2& normal) {
        // when point is on rect's conner, return normal = 0
        if ((p.x == r.pos.x || p.x == r.pos.x + r.size.x) &&
            (p.y == r.pos.y || p.y == r.pos.y + r.size.y)) {
            normal = glm::vec2(0);
            return true;
        }
        if (p.y == r.pos.y) {
            normal = glm::vec2(0, -1);
            return (p.x >= r.pos.x && p.x <= r.pos.x + r.size.x);
        }
        if (p.y == r.pos.y + r.size.y) {
            normal = glm::vec2(0, 1);
            return (p.x >= r.pos.x && p.x <= r.pos.x + r.size.x);
        }
        if (p.x == r.pos.x) {
            normal = glm::vec2(-1, 0);
            return (p.y >= r.pos.y && p.y <= r.pos.y + r.size.y);
        }
        if (p.x == r.pos.x + r.size.x) {
            normal = glm::vec2(1, 0);
            return (p.y >= r.pos.y && p.y <= r.pos.y + r.size.y);
        }
        return 0;
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

    /// @brief 
    /// @param l ray data
    /// @param r rect data
    /// @param contact_data contact point,time,normal,... 
    /// @return true when ray hit, not when 'touch'  rect
    bool ray_vs_rect(const ray& l, const rect& r, contact* contact_data) {
        //* default, hit=false
        *contact_data = { false,l.end(),glm::vec2(0),1 };

        //if (point_in_rect(l.pos, r)) return false;
        //if (l.dir == glm::vec2(0)) return false;

        //* time for ray to go pass x/y bound of the rectangle
        //* if  near xy < far xy  -> ray hit
        std::cout << "check start";
        glm::vec2 t_near = (r.pos - l.pos) / l.dir;
        glm::vec2 t_far = (r.pos + r.size - l.pos) / l.dir;

        // return false if ray is on the same line as one of rect's edge
        // -> allow sliding
        if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
        if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;

        // sort distance
        if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
        if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

        // check
        if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

        float t_hit_near = std::max(t_near.x, t_near.y);
        float t_hit_far = std::min(t_far.x, t_far.y);

        //* contact at oposite direction
        if (t_hit_far <= 0) return false;

        //* contact at point farther than ray length
        if (t_hit_near > 1) return false;

        contact_data->hit = true;
        contact_data->t_near = t_hit_near;
        contact_data->t_far = t_hit_far;
        contact_data->pos = l.pos + t_hit_near * l.dir;
        if (t_near.x > t_near.y)
            if (l.dir.x < 0)
                contact_data->normal = { 1, 0 };
            else
                contact_data->normal = { -1, 0 };
        else if (t_near.x < t_near.y) {
            if (l.dir.y < 0)
                contact_data->normal = { 0, 1 };
            else
                contact_data->normal = { 0, -1 };
        }

        //  if contact happen at conner ( tnear.x = tnear.y ), normal = 0, collision wont resolve;
        //  if t_near = t_far, contact happen at conner
        return true;
    }

}