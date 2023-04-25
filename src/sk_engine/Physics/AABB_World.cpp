#include "AABB_World.h"

#include <iostream>
#include <cmath>
#include <algorithm>

#include <game/Entity.h>

#include <Graphics/2D_Renderer.h>

#include <Common/sk_time.h>

namespace sk_physic2d {

    void AABB_World::Hint_WorldBound(glm::vec4 bound) { world_bound = bound; }
    void AABB_World::Hint_WorldBound(glm::vec2 center, float size) { world_bound = { center - glm::vec2(size) * 0.5f,center + glm::vec2(size) * 0.5f }; }

    void AABB_World::Init() {
        m_Body.reserve(100);
        solids.reserve(100);
        actors.reserve(10);
        quad_tree.Init(irect::irect_fbound(world_bound));

        std::cout << "world_bound: ";
        std::cout << world_bound.x << " ";
        std::cout << world_bound.y << " ";
        std::cout << world_bound.z << " ";
        std::cout << world_bound.w << "\n";
        std::cout << quad_tree.root.node_rect.bound.x << " ";
        std::cout << quad_tree.root.node_rect.bound.y << " ";
        std::cout << quad_tree.root.node_rect.bound.z << " ";
        std::cout << quad_tree.root.node_rect.bound.w << "\n";

    }
    void AABB_World::Clear() {
        m_Body.clear();
        quad_tree.Clear();
    }

    //* add and remove bodies ---------------------------------------------------------------------------------

    int AABB_World::Create_Body(const Body_Def& def) {
        body_added_or_removed = true;
        int id;
        if (!this->avaiable_mbody_index.empty()) {
            id = this->avaiable_mbody_index.back();
            avaiable_mbody_index.pop_back();
        }
        else {
            m_Body.emplace_back();
            id = m_Body.size() - 1;
        }
        body_counts++;

        m_Body[id] = Body(def);

        quad_tree.AddValue(m_Body[id].RECT, id);
        return id;
    }
    Body* AABB_World::Get_Body(const int index) {
        return &m_Body.at(index);
    }
    void AABB_World::Remove_Body(const int index) {

        if (m_Body.size() <= index || index < 0) return;
        if (!m_Body[index].is_active) return;

        body_added_or_removed = true;

        body_counts--;
        m_Body[index].is_active = false;
        quad_tree.RemoveValue(index);

        avaiable_mbody_index.push_back(index);
    }

    //* query and reslove --------------------------------------------------------------------------------------------------

    void AABB_World::GetSABodyList() {
        solids.clear();
        actors.clear();
        if (!m_Body.empty()) {
            for (int i = 0; i <= m_Body.size() - 1;i++) {
                if (m_Body[i].is_active && CheckTag(m_Body[i].tag, PHY_MOVEABLE) && CheckTag(m_Body[i].tag, PHY_SOLID))
                    solids.push_back(i);
                if (m_Body[i].is_active && CheckTag(m_Body[i].tag, PHY_ACTOR))
                    actors.push_back(i);
            }
        }
    }
    void AddToUpdateList(int id) {

    }

    void AABB_World::ResolveActor(int id) {
        Body& a_body = m_Body[id];

        //std::cout << "Resolving Actor, id: " << id << "  " << "ptr: " << &m_Body[id] << '\n';
        //std::cout << "New velocity: " << a_body.velocity.x << " " << a_body.velocity.y << '\n';
        //postion equation: 
        // s = s + v*t + 0.5*a*t*t
        // s: position
        // v: velocity
        // a: acceleration 
        /*glm::vec2 move_amount =
            a_body.RECT.offset +
            a_body.velocity * sk_time::delta_time * (float)INTCOORD_PRECISION;*/
        if (a_body.velocity.y > 100) std::cout << "too fast \n";
        glm::vec2 move_amount =
            a_body.RECT.offset +
            a_body.prev_velocity * sk_time::delta_time * (float)INTCOORD_PRECISION +
            (a_body.velocity - a_body.prev_velocity) * 0.5f * sk_time::delta_time * sk_time::delta_time * (float)INTCOORD_PRECISION
            ;

        // round away from 0, use to query
        int x_query = (move_amount.x < 0) ? floor(move_amount.x) : ceil(move_amount.x);
        int y_query = (move_amount.y < 0) ? floor(move_amount.y) : ceil(move_amount.y);
        // round toward, use to move body
        int x_move = trunc(move_amount.x);
        int y_move = trunc(move_amount.y);

        a_body.RECT.offset = move_amount - glm::vec2(x_move, y_move);

        irect query_rect = a_body.RECT.extend_(x_query, y_query);

        std::vector<int> possible_collision = Query(query_rect);

        // draw query rects
        if (enable_debug_draw)
            for (int index : possible_collision)
                sk_graphic::Renderer2D_AddBBox(m_Body[index].RECT.true_bound(), 2, { 1,1,1,1 });

        for (int s_index : possible_collision)
            if (m_Body[s_index].is_active && CheckTag(m_Body[s_index].tag, etag::PHY_SOLID) && !CheckTag(m_Body[s_index].tag, etag::PHY_ONE_WAY)) {
                if (a_body.RECT.overlap(m_Body[s_index].RECT)) std::cout << "Physic Error: rect overlap\n";
            }
        if (x_query != 0) { // solve x
            //get direction
            int xdir = 1;
            if (x_move < 0) {
                xdir = -1;
                x_move = -x_move;
            }
            // solve for each static body
            for (int s_index : possible_collision) if (m_Body[s_index].is_active && CheckTag(m_Body[s_index].tag, etag::PHY_SOLID)) {
                if (CheckTag(m_Body[s_index].tag, etag::PHY_ONE_WAY)) continue; // skip solve in x if is oneway
                Body& s_body = m_Body[s_index];

                if (s_body.RECT.bound.w > a_body.RECT.bound.y && a_body.RECT.bound.w > s_body.RECT.bound.y) { // y overlap
                    int distant = std::max(
                        a_body.RECT.bound.x - s_body.RECT.bound.z,
                        s_body.RECT.bound.x - a_body.RECT.bound.z
                    );
                    if (x_move >= distant) {
                        x_move = distant;
                        a_body.RECT.offset.x = 0;
                        a_body.velocity.x = 0;
                    }
                }
            }
            // after solve, apply movement
            a_body.RECT.bound.x += x_move * xdir;
            a_body.RECT.bound.z += x_move * xdir;
        }
        if (y_query != 0) { // solve y
            //get direction
            int ydir = 1;
            if (y_move < 0) {
                ydir = -1;
                y_move = -y_move;
            }
            // solve for each static body
            for (int s_index : possible_collision) if (m_Body[s_index].is_active && CheckTag(m_Body[s_index].tag, etag::PHY_SOLID)) {
                Body& s_body = m_Body[s_index];

                if (s_body.RECT.bound.z > a_body.RECT.bound.x && a_body.RECT.bound.z > s_body.RECT.bound.x) { // x overlap
                    if (CheckTag(m_Body[s_index].tag, etag::PHY_ONE_WAY) && a_body.RECT.bound.y < s_body.RECT.bound.w) continue;
                    int distant = std::max(
                        a_body.RECT.bound.y - s_body.RECT.bound.w,
                        s_body.RECT.bound.y - a_body.RECT.bound.w
                    );
                    if (y_move >= distant) {
                        y_move = distant;
                        a_body.RECT.offset.y = 0;
                        a_body.velocity.y = 0;
                    }
                }
            }
            // after solve, apply movement
            a_body.RECT.bound.y += y_move * ydir;
            a_body.RECT.bound.w += y_move * ydir;
        }

        if (m_Body[id].entity != nullptr)
            for (int index : possible_collision) {
                if (CheckTag(m_Body[index].tag, etag::PHY_TRIGGER) && id != index && a_body.RECT.overlap(m_Body[index].RECT)) {
                    m_Body[id].entity->OnTrigger(m_Body[index].tag);
                    if (m_Body[index].entity)
                        m_Body[id].entity->OnTrigger(m_Body[index].entity);
                }
            }
        a_body.prev_velocity = a_body.velocity;
    }
    void AABB_World::ResolveSolid(int id) {

    }

    //* check touching and raycasting --------------------------------------------------------------------------------------

    /// @brief check touching solid, using integer based bounding box 
    bool AABB_World::TouchSolid_ibound(glm::ivec4 ibound) {
        auto possible_collision = Query(irect(ibound));
        for (int body_id : possible_collision) if (m_Body[body_id].is_active && CheckTag(m_Body[body_id].tag, etag::PHY_SOLID))
            return true;
        return 0;
    }

    bool AABB_World::BoxCast(glm::ivec4 ibound, uint64_t tag, uint64_t null_tag) {
        auto possible_collision = Query(irect(ibound));
        for (int body_id : possible_collision)
            if (m_Body[body_id].is_active && (m_Body[body_id].tag & tag) == tag && (m_Body[body_id].tag & null_tag) == 0)
                return true;
        return 0;
    }

//* Debug draw and Update ----------------------------------------------------------------------------------------------

    void AABB_World::Draw() {
        if (!enable_debug_draw) return;
        for (auto& body : this->m_Body)
            if (body.is_active) {
                sk_graphic::Renderer2D_AddBBox(body.RECT.true_bound(0), 1, glm::vec4(0, 1, 0, 1));
            }
        quad_tree.Draw();
    }

    void AABB_World::Update() {
        GetSABodyList();
        for (int index : actors) ResolveActor(index);
        for (int index : solids) ResolveSolid(index);

        for (int index : actors) quad_tree.UpdateValue(m_Body[index].RECT, index);
        //for (int index : solids) quad_tree.UpdateValue(m_Body[index].RECT, index);
    }
}