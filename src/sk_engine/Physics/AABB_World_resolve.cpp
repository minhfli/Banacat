#include "AABB_World.h"

#include <game/Entity.h>

#include <sk_engine/Graphics/Graphics.h>

#include <sk_engine/Common/sk_time.h>

#include <algorithm>

namespace sk_physic2d {
    void AABB_World::MoveActorX(Body& a_body, int x_move, bool force) {
        if (x_move == 0 && a_body.RECT.offset.x == 0) return;
        int dir = (x_move == 0) ? ((a_body.RECT.offset.x < 0) ? -1 : 1) : ((x_move < 0) ? -1 : 1);
        if (x_move < 0) x_move = -x_move;

        irect query_rect = a_body.RECT.extend_(x_move * dir, 0);
        if (dir == -1) query_rect.bound.x -= 1;
        if (dir == 1) query_rect.bound.z += 1;

        auto possible_collision = Query(query_rect);

        for (int s_index : possible_collision) {
            Body& s_body = m_Body[s_index];
            if (!s_body.is_active) continue;
            if (!CheckTag(s_body.tag, etag::PHY_SOLID)) continue;
            if (CheckTag(s_body.tag, etag::PHY_ONE_WAY)) continue;
            if (force && a_body.entity != nullptr) {
                a_body.entity->OnSquish();
                return;
            }
            int distant = std::max(
                a_body.RECT.bound.x - s_body.RECT.bound.z,
                s_body.RECT.bound.x - a_body.RECT.bound.z
            );
            if (x_move >= distant) {
                x_move = distant;
                a_body.RECT.offset.x = 0;
                if (dir == 1)
                    a_body.velocity.x = std::min(.0f, a_body.velocity.x);
                else
                    a_body.velocity.x = std::max(.0f, a_body.velocity.x);
            }
        }
        a_body.RECT.bound.x += x_move * dir;
        a_body.RECT.bound.z += x_move * dir;
    }
    void AABB_World::MoveActorY(Body& a_body, int y_move, bool force) {
        if (y_move == 0 && a_body.RECT.offset.y == 0) return;
        int dir = (y_move == 0) ? ((a_body.RECT.offset.y < 0) ? -1 : 1) : ((y_move < 0) ? -1 : 1);
        if (y_move < 0) y_move = -y_move;

        irect query_rect = a_body.RECT.extend_(0, y_move * dir);
        if (dir == -1) query_rect.bound.y -= 1;
        if (dir == 1) query_rect.bound.w += 1;
        auto possible_collision = Query(query_rect);

        for (int s_index : possible_collision) {
            Body& s_body = m_Body[s_index];
            if (!s_body.is_active) continue;
            if (!CheckTag(s_body.tag, etag::PHY_SOLID)) continue;
            if (CheckTag(s_body.tag, etag::PHY_ONE_WAY) && a_body.RECT.bound.y < s_body.RECT.bound.w) continue;
            if (force && a_body.entity != nullptr) { a_body.entity->OnSquish(); return; }
            int distant = std::max(
                a_body.RECT.bound.y - s_body.RECT.bound.w,
                s_body.RECT.bound.y - a_body.RECT.bound.w
            );
            if (y_move >= distant) {
                y_move = distant;
                a_body.RECT.offset.y = 0;
                if (dir == 1)
                    a_body.velocity.y = std::min(.0f, a_body.velocity.y);
                else
                    a_body.velocity.y = std::max(.0f, a_body.velocity.y);
            }
        }
        a_body.RECT.bound.y += y_move * dir;
        a_body.RECT.bound.w += y_move * dir;
    }
    void AABB_World::MoveSolidX(Body& s_body, int x_move, const std::vector <Body*>& riding_list) {
        if (x_move == 0) return;
        int dir = (x_move < 0) ? -1 : 1; x_move *= dir;
        irect query_rect = s_body.RECT.extend_(x_move * dir, 0);
        auto possible_collision = Query(query_rect);
        s_body.RECT.bound.x += x_move * dir;
        s_body.RECT.bound.z += x_move * dir;
        s_body.is_active = false;

        for (auto a_body : riding_list) {
            if (!query_rect.overlap(a_body->RECT))  // ignore carring if actor get pushed 
                MoveActorX(*a_body, x_move, 0);
        }
        for (int a_index : possible_collision) {
            Body& a_body = m_Body[a_index];
            if (!a_body.is_active) continue;
            if (!CheckTag(a_body.tag, etag::PHY_ACTOR)) continue;
            a_body.RECT.offset.x = s_body.RECT.offset.x;
            if (x_move > 0)                         // push actor
                MoveActorX(a_body, query_rect.bound.z - a_body.RECT.bound.x, true);
            else
                MoveActorX(a_body, query_rect.bound.x - a_body.RECT.bound.z, true);
        }
        s_body.is_active = true;
    }
    void AABB_World::MoveSolidY(Body& s_body, int y_move, const std::vector <Body*>& riding_list) {
        if (y_move == 0) return;
        int dir = (y_move < 0) ? -1 : 1; y_move *= dir;
        irect query_rect = s_body.RECT.extend_(y_move * dir, 0);
        auto possible_collision = Query(query_rect);
        s_body.RECT.bound.y += y_move * dir;
        s_body.RECT.bound.w += y_move * dir;
        s_body.is_active = false;

        for (auto a_body : riding_list) {
            if (!query_rect.overlap(a_body->RECT))  // ignore carring if actor get pushed 
                MoveActorY(*a_body, y_move, 0);
        }
        for (int a_index : possible_collision) {
            Body& a_body = m_Body[a_index];
            if (!a_body.is_active) continue;
            if (!CheckTag(a_body.tag, etag::PHY_ACTOR)) continue;
            a_body.RECT.offset.y = s_body.RECT.offset.y;
            if (y_move > 0)                         // push actor
                MoveActorY(a_body, query_rect.bound.w - a_body.RECT.bound.y, true);
            else
                MoveActorY(a_body, query_rect.bound.y - a_body.RECT.bound.w, true);
        }
        s_body.is_active = true;
    }

    void AABB_World::ResolveActor(int id) {
        Body& a_body = m_Body[id];
        //* check overlap ------------------------------------------------------------------------------------
        std::vector<int> possible_collision = Query(a_body.RECT);
        if (enable_debug_draw)
            for (int index : possible_collision)
                sk_graphic::Renderer2D_AddBBox(m_Body[index].RECT.true_bound(), 2, { 1,1,1,1 });
        if (a_body.entity != nullptr) for (int index : possible_collision) {
            Body& t_body = m_Body[index];
            if (!t_body.is_active) continue;
            if (CheckTag(t_body.tag, etag::PHY_TRIGGER)) { // check trigger
                if (CheckTag(t_body.tag, etag::PHY_DIR_U) && a_body.velocity.y > 0) continue;
                if (CheckTag(t_body.tag, etag::PHY_DIR_D) && a_body.velocity.y < -1) continue;
                if (CheckTag(t_body.tag, etag::PHY_DIR_L) && a_body.velocity.x < 0) continue;
                if (CheckTag(t_body.tag, etag::PHY_DIR_R) && a_body.velocity.x > 0) continue;
                if (id != index) {
                    a_body.entity->OnTrigger(t_body.tag);
                    if (t_body.entity) {
                        a_body.entity->OnTrigger(t_body.entity);
                        t_body.entity->OnTrigger(a_body.entity);
                        t_body.entity->OnTrigger(a_body.tag);
                    }
                }
                continue;
            }
            if (CheckTag(t_body.tag, etag::PHY_SOLID) && !CheckTag(t_body.tag, etag::PHY_ONE_WAY))
                a_body.entity->OnSquish();
        }
        //* movement ----------------------------------------------------------------------------------------------
        glm::vec2 move_amount =
            a_body.RECT.offset +
            a_body.velocity * sk_time::delta_time * (float)INTCOORD_PRECISION +
            a_body.move_amount * (float)INTCOORD_PRECISION;
        a_body.move_amount = glm::vec2(0);

        // round toward, use to move body
        int x_move = trunc(move_amount.x);
        int y_move = trunc(move_amount.y);
        a_body.RECT.offset = move_amount - glm::vec2(x_move, y_move);

        MoveActorX(a_body, x_move, 0);
        MoveActorY(a_body, y_move, 0);
    }
    void AABB_World::ResolveSolid(int id) {
        Body& s_body = m_Body[id];
        glm::vec2 move_amount =
            s_body.RECT.offset +
            s_body.velocity * sk_time::delta_time * (float)INTCOORD_PRECISION +
            s_body.move_amount * (float)INTCOORD_PRECISION;
        s_body.move_amount = glm::vec2(0);

        // round toward, use to move body
        int x_move = trunc(move_amount.x);
        int y_move = trunc(move_amount.y);

        s_body.RECT.offset = move_amount - glm::vec2(x_move, y_move);

        irect query_rect = s_body.RECT.expand_(1, 1);
        std::vector<int> possible_riding = Query(query_rect);

        std::vector<Body*> riding_actor; riding_actor.reserve(possible_riding.size());
        for (int index : possible_riding) {
            Body& a_body = m_Body[index];

            if (CheckTag(a_body.tag, etag::PHY_ACTOR) && a_body.entity != nullptr)
                if (a_body.entity->IsRiding(&s_body)) {
                    riding_actor.emplace_back(&a_body);
                    if (s_body.entity != nullptr) s_body.entity->OnRiding(a_body.entity);
                    if (a_body.entity != nullptr) a_body.entity->OnRiding(s_body.entity);
                }
        }
        MoveSolidX(s_body, x_move, riding_actor);
        MoveSolidY(s_body, y_move, riding_actor);

    }
}