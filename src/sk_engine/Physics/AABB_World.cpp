#include "AABB_World.h"

#include <iostream>
#include <cmath>
#include <algorithm>


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

    Body* AABB_World::Create_Body(const Body_Def& def, int* index) {
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

        if (index != nullptr) *index = id;
        m_Body[id] = Body(def);

        quad_tree.AddValue(m_Body[id].RECT, id);
        return &m_Body[id];

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
    void AABB_World::Remove_Body(Body* body) {
        int index = body - &m_Body.front();
        Remove_Body(index);
    }

    //* query and reslove --------------------------------------------------------------------------------------------------

    void AABB_World::GetSABodyList() {
        solids.clear();
        actors.clear();
        if (!m_Body.empty()) {
            for (int i = 0; i <= m_Body.size() - 1;i++) {
                if (m_Body[i].is_solid() && m_Body[i].velocity != glm::vec2(0))
                    solids.push_back(i);
                if (m_Body[i].is_actor())
                    actors.push_back(i);
            }
        }
    }
    void AABB_World::ResolveActor(int id) {
        Body& a_body = m_Body[id];

        glm::vec2 move_amount = a_body.RECT.offset + a_body.velocity * sk_time::delta_time * (float)INTCOORD_PRECISION;

        // round away from 0, use to query
        int x_query = (move_amount.x < 0) ? floor(move_amount.x) : ceil(move_amount.x);
        int y_query = (move_amount.y < 0) ? floor(move_amount.y) : ceil(move_amount.y);
        // round toward, use to move body
        int x_move = trunc(move_amount.x);
        int y_move = trunc(move_amount.y);

        a_body.RECT.offset = move_amount - glm::vec2(x_move, y_move);

        irect query_rect = a_body.RECT.extend_(x_query, y_query);

        std::vector<int> possible_collision = Query(query_rect);

        for (int index : possible_collision) {
            sk_graphic::Renderer2D_AddBBox(m_Body[index].RECT.true_bound(), 2, { 1,1,1,1 });
        }

        if (x_query != 0) { // solve x
            //get direction
            int xdir = 1;
            if (x_move < 0) {
                xdir = -1;
                x_move = -x_move;
            }
            // solve for each static body
            for (int s_index : possible_collision) if (m_Body[s_index].is_solid()) {
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
            for (int s_index : possible_collision) if (m_Body[s_index].is_solid()) {
                Body& s_body = m_Body[s_index];

                if (s_body.RECT.bound.z > a_body.RECT.bound.x && a_body.RECT.bound.z > s_body.RECT.bound.x) { // x overlap
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
    }
    void AABB_World::ResolveSolid(int id) {
    }


    void AABB_World::Draw() {
        for (auto& body : this->m_Body)
            if (body.is_active) {
                sk_graphic::Renderer2D_AddBBox(body.RECT.true_bound(0), 1, glm::vec4(0, 1, 0, 1));
            }
        quad_tree.Draw();
    }

    void AABB_World::Update() {
        if (sk_time::delta_time > 0.5f) {
            std::cout << "Error: Big delta time \n";
            return;
        }
        GetSABodyList();
        for (int index : actors) ResolveActor(index);
        for (int index : solids) ResolveSolid(index);

        for (int index : actors) quad_tree.UpdateValue(m_Body[index].RECT, index);
        //for (int index : solids) quad_tree.UpdateValue(m_Body[index].RECT, index);
    }
}