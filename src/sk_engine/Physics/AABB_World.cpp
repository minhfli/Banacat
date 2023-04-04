#include "AABB_World.h"

#include <iostream>

#include <Graphics/2D_Renderer.h>

#include <Common/sk_time.h>

namespace sk_physic2d {

    void AABB_World::Hint_WorldBound(glm::vec4 bound) { world_bound = rect(bound); }
    void AABB_World::Hint_WorldBound(glm::vec2 center, float size) { world_bound = rect(center, glm::vec2(size)); }

    void AABB_World::Init() {
        m_Body.reserve(100);
        solids.reserve(100);
        actors.reserve(10);
        quad_tree.Init(world_bound);
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

        rect query_rect(
            a_body.RECT.pos + a_body.velocity * 0.5f * sk_time::delta_time,
            a_body.RECT.hsize * 2.0f + a_body.velocity * sk_time::delta_time
        );
        std::vector<int> possible_collision = Query(query_rect);

        // apply gravity

        glm::vec2 newpos = a_body.RECT.pos + a_body.velocity * sk_time::delta_time;

        if (std::abs(a_body.velocity.x) > 0.01f) {   // solve x
            float tx = sk_time::delta_time;
            for (int index : possible_collision) if (m_Body[index].is_solid()) {
                Body& s_body = m_Body[index];
                glm::vec4 s_bound = s_body.RECT.bound(a_body.RECT.hsize);
                if (a_body.RECT.pos.y > s_bound.y && a_body.RECT.pos.y < s_bound.w) {   // y overlap
                    float tnear = (s_bound.x - a_body.RECT.pos.x) / a_body.velocity.x;
                    float tfar = (s_bound.z - a_body.RECT.pos.x) / a_body.velocity.x;

                    float tmin = std::min(tnear, tfar);
                    if (tmin > -0.01f && tx > tmin) {
                        if (tnear < tfar)
                            newpos.x = s_bound.x;
                        else
                            newpos.x = s_bound.z;
                        tx = tmin;
                    }
                }
            }
            if (tx < sk_time::delta_time) a_body.velocity.x = 0;
        }
        if (std::abs(a_body.velocity.y) > 0.01f) {   // solve y
            float ty = sk_time::delta_time;
            for (int index : possible_collision) if (m_Body[index].is_solid()) {
                Body& s_body = m_Body[index];
                glm::vec4 s_bound = s_body.RECT.bound(a_body.RECT.hsize);
                if (a_body.RECT.pos.x > s_bound.x && a_body.RECT.pos.x < s_bound.z) {   // x overlap
                    float tnear = (s_bound.y - a_body.RECT.pos.y) / a_body.velocity.y;
                    float tfar = (s_bound.w - a_body.RECT.pos.y) / a_body.velocity.y;

                    float tmin = std::min(tnear, tfar);
                    if (tmin > -0.01f && ty > tmin) {
                        if (tnear < tfar)
                            newpos.y = s_bound.y;
                        else
                            newpos.y = s_bound.w;
                        ty = tmin;
                    }
                }
            }
            if (ty < sk_time::delta_time) a_body.velocity.y = 0;
        }
        a_body.RECT.pos = newpos;
        a_body.velocity.y -= 10 * sk_time::delta_time;

        std::cout << "actor:    " << id << '\n';
        std::cout << "velocity: " << a_body.velocity.x << " " << a_body.velocity.y << '\n';
        std::cout << "position: " << a_body.RECT.pos.x << " " << a_body.RECT.pos.y << '\n';
    }
    void AABB_World::ResolveSolid(int id) {
    }


    void AABB_World::Draw() {
        for (auto& body : this->m_Body)
            if (body.is_active) {
                sk_graphic::Renderer2D_AddBBox(body.RECT.bound(), 1, glm::vec4(0, 1, 0, 1));
            }
        quad_tree.Draw();
    }

    void AABB_World::Update() {
        if (sk_time::delta_time > 0.5f) {
            std::cout << "Error: Big delta time \n";
            return;
        }
        std::cout << (int)(-1) / 2;
        GetSABodyList();
        for (int index : actors) ResolveActor(index);
        for (int index : solids) ResolveSolid(index);

        for (int index : actors) quad_tree.UpdateValue(m_Body[index].RECT, index);
        //for (int index : solids) quad_tree.UpdateValue(m_Body[index].RECT, index);
    }
}