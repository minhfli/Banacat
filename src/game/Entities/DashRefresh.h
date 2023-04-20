#pragma once

#include "../Entity.h"

#include <sk_engine/Physics/AABB.h>
#include <sk_engine/Graphics/Texture2D.h>

class DashRefresh:public Entity {
    public:
    DashRefresh() {}
    ~DashRefresh() {}

    glm::vec2 pos;
    sk_physic2d::AABB_World* physic_world;
    int trigger_body;
    sk_graphic::Texture2D m_texture;

    bool is_active = true;
    float cooldown_time = 1.0f;
    float last_triggered_time;

    void OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) override;
    void OnCreate(Area* area, Level* level) override {}
    void OnDestroy() override;

    void OnActive() override {}
    void OnDeActive() override {}

    void OnTrigger(Entity* trigger) override {}
    void OnTrigger(uint64_t trigger_tag) override {}

    void Start() override {}
    void Update() override;
    void LateUpdate() override {}
    void Draw() override;
};


