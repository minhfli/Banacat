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
        solids.reserve(1000);
        actors.reserve(100);
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
        avaiable_mbody_index.clear();
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



    //* check touching and raycasting --------------------------------------------------------------------------------------

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
        for (int index : solids) quad_tree.UpdateValue(m_Body[index].RECT, index);
    }
}