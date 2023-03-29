#include "AABB_World.h"
#include <Graphics/2D_Renderer.h>

namespace sk_physic2d {

    void AABB_World::Hint_WorldBound(glm::vec4 bound) { world_bound = rect(bound); }
    void AABB_World::Hint_WorldBound(glm::vec2 center, float size) { world_bound = rect(center, glm::vec2(size)); }

    void AABB_World::Init() {
        m_Body.reserve(100);
        quad_tree.Init(world_bound);
    }

    Body* AABB_World::Create_Body(const Body_Def& def, int* index) {
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

        body_counts--;
        m_Body[index].is_active = false;
        quad_tree.RemoveValue(index);

        avaiable_mbody_index.push_back(index);
    }
    void AABB_World::Remove_Body(Body* body) {
        int index = body - &m_Body.front();
        Remove_Body(index);
    }

    void AABB_World::Draw() {
        for (auto& body : this->m_Body)
            if (body.is_active) {
                sk_graphic::Renderer2D_AddBBox(body.RECT.bound(), 1, glm::vec4(0, 1, 0, 1));
            }
        quad_tree.Draw();
    }
}