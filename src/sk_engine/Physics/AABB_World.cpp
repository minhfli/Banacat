#include "AABB_World.h"

namespace sk_physic2d {
    void AABB_World::Init() {
        m_Body.reserve(50);
    }

    Body* AABB_World::Create_Body(int* index) {
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
        return &m_Body.at(id);

    }
    void AABB_World::Remove_Body(int& index) {
        if (m_Body.size() <= index || index < 0) return;
        if (!m_Body[index].is_active) return;

        m_Body[index].is_active = false;
        avaiable_mbody_index.push_back(index);
    }
    void AABB_World::Remove_Body(Body* body) {
        int index = body - &m_Body.front();
        Remove_Body(index);
    }
}