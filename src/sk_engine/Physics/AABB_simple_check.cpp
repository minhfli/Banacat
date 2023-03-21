#include "AABB_collision_functions.h"

namespace sk_physic2d {
    bool point_in_rect(const glm::vec2& p, const rect& r, contact* contact_data) {
        if (contact_data == nullptr)
            return (
                p.x >= r.pos.x &&
                p.y >= r.pos.y &&
                p.x <= r.pos.x + r.size.x &&
                p.y <= r.pos.y + r.size.y
            );

        contact_data->hit = 0;

        if (!( //if not hit
            p.x >= r.pos.x &&
            p.y >= r.pos.y &&
            p.x <= r.pos.x + r.size.x &&
            p.y <= r.pos.y + r.size.y
            )) return false;
        contact_data->hit = 1;

        // contact_data.t_near is now used to calcutate the nearest wya to resolve colltion
        float x_near = p.x - r.pos.x;
        float y_near = p.y - r.pos.y;
        float x_far = r.pos.x + r.size.x - p.x;
        float y_far = r.pos.y + r.size.y - p.y;

        if (x_near < x_far) {
            contact_data->normal = glm::vec2(-1, 0);
            contact_data->pos = glm::vec2(p.x - x_near, p.y);
        }
        else {
            contact_data->normal = glm::vec2(1, 0);
            contact_data->pos = glm::vec2(p.x + x_far, p.y);
        }

        if (std::min(y_near, y_far) > std::min(x_near, x_far)) return true;

        if (y_near < y_far) {
            contact_data->normal = glm::vec2(0, -1);
            contact_data->pos = glm::vec2(p.x, p.y - y_near);
        }
        else {
            contact_data->normal = glm::vec2(0, 1);
            contact_data->pos = glm::vec2(p.x, p.y + y_far);
        }
        return true;
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
        if (t_hit_near < 0) return false;

        //* contact at point farther than ray length
        if (t_hit_near > 1) return false;

        //* if ray go throuh conner ( tnear.x = tnear.y ), normal = 0, collision wont resolve;
        if (t_hit_near == t_hit_far) return false;

        contact_data->hit = true;
        contact_data->t_near = t_hit_near;
        contact_data->t_far = t_hit_far;
        contact_data->pos = l.pos + t_hit_near * l.dir;
        if (t_near.x > t_near.y) {
            if (l.dir.x < 0)
                contact_data->normal = { 1, 0 };
            else
                contact_data->normal = { -1, 0 };
        }
        else if (t_near.x < t_near.y) {
            if (l.dir.y < 0)
                contact_data->normal = { 0, 1 };
            else
                contact_data->normal = { 0, -1 };
        }
        else {
            //contact happen at conner
            if (l.dir.y < 0)
                contact_data->normal = { 0, 1 };
            else
                contact_data->normal = { 0, -1 };
        }
        return true;
    }
    /// @brief 
    /// @param r1 rect1
    /// @param r2 rect2, must be static
    /// @param contact_data contact point,time,normal,... 
    /// @return true when ray hit, not when 'touch'  rect
    bool swept_rect_vs_rect(const rect& r1, const rect& r2, contact* contact_data, const float delta_time) {
        //* default, hit=false
        *contact_data = {
            false,
            r1.center() + r1.velocity * delta_time,
            glm::vec2(0),
            1,
            1
        };


        if (r1.velocity == glm::vec2(0)) return false;

        ray r1_srink(r1.center(), r1.velocity * delta_time);
        rect r2_expand(r2.pos - r1.size / 2.0f, r2.size + r1.size);

        return ray_vs_rect(r1_srink, r2_expand, contact_data);
    }
        /// @brief 
        /// @param r1 
        /// @param r2 
        /// @param contact_data 
        /// @return 
    bool colli_rect_vs_rect(const rect& r1, const rect& r2, contact* contact_data) {
        *contact_data = {
            false,
            r1.center(),
            glm::vec2(0),
            0,
            0
        };
        rect r2_expand(r2.pos - r1.size / 2.0f, r2.size + r1.size);
        return point_in_rect(r1.center(), r2_expand, contact_data);
    }

}