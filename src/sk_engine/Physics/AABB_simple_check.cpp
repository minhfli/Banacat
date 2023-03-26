#include "AABB_collision_functions.h"

namespace sk_physic2d {
    bool point_in_rect(const glm::vec2& p, const glm::vec4& r, contact* contact_data) {
        if (contact_data == nullptr)
            return (
                p.x >= r.x &&
                p.y >= r.y &&
                p.x <= r.z &&
                p.y <= r.w
            );

        contact_data->hit = 0;

        if (!( //if not hit
            p.x >= r.x &&
            p.y >= r.y &&
            p.x <= r.z &&
            p.y <= r.w
            )) return false;
        contact_data->hit = 1;

        // contact_data.t_near is now used to calcutate the nearest wya to resolve colltion
        float x_near = p.x - r.x;
        float y_near = p.y - r.y;
        float x_far = r.z - p.x;
        float y_far = r.w - p.y;

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

    bool rect_contain(const glm::vec4& r1, const glm::vec4& r2) {
        return (
            r1.x >= r2.x &&
            r1.y >= r2.y &&
            r1.z <= r2.z &&
            r1.w <= r2.w
        );
    }

    bool rect_overlap(const glm::vec4& r1, const glm::vec4& r2) {
        return (
            r1.z >= r2.x &&
            r2.z >= r1.x &&
            r1.w >= r2.y &&
            r2.w >= r1.y
        );
    }

    /// @brief 
    /// @param l ray data
    /// @param r rect data
    /// @param contact_data contact point,time,normal,... 
    /// @return true when ray hit, not when 'touch'  rect
    bool ray_vs_rect(const ray& l, const glm::vec4& r, contact* contact_data) {
        //* default, hit=false
        *contact_data = { false,l.end(),glm::vec2(0),1 };

        if (point_in_rect(l.pos, r)) return false;
        if (l.dir == glm::vec2(0)) return false;

        //* time for ray to go pass x/y bound of the rectangle
        //* if  near xy < far xy  -> ray hit
        glm::vec2 t_near = (r.xy() - l.pos) / l.dir;
        glm::vec2 t_far = (r.zw() - l.pos) / l.dir;

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
    /// @param r1offset offset(swept value) of rect 1
    /// @param contact_data contact point,time,normal,... 
    /// @return true when ray hit, not when 'touch'  rect
    bool swept_rect_vs_rect(const rect& r1, const rect& r2, glm::vec2 r1offset, contact* contact_data) {
        //* default, hit=false
        *contact_data = {
            false,
            r1.pos + r1offset,
            glm::vec2(0),
            1,
            1
        };


        if (r1offset == glm::vec2(0)) return false;

        ray r1_srink(r1.pos, r1offset);
        glm::vec4 r2_expand = r2.bound(r1.hsize);

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
            r1.pos,
            glm::vec2(0),
            0,
            0
        };
        glm::vec4 r2_expand = r2.bound(r1.hsize);
        return point_in_rect(r1.pos, r2_expand, contact_data);
    }

}