#pragma once

#include <sk_engine/Physics/AABB.h>
#include "../Entity.h"
#include <GLM/glm.hpp>
#include "Animation.h"

class Area;
class Level;

class Fish_biscuit : public Entity {
    public:
    Fish_biscuit() {}
    ~Fish_biscuit() {}

    int trigger_body;
    sk_physic2d::Body* m_body;
    nlohmann::json* active_field_instace = nullptr;
    int un_active_tick = -10000;
    Animation ani;
    int sfx_collect;

    void OnJsonCreate(Area* area, Level* level, nlohmann::json& jentity) override;
    void OnCreate(Area* area, Level* level) override {}
    void OnDestroy() override;

    void OnActive() override {}
    void OnDeActive() override {}

    void OnTrigger(Entity* trigger) override {}
    void OnTrigger(uint64_t trigger_tag) override;

    void Start() override {}
    void Update() override;
    void LateUpdate() override {}
    void Draw() override;
};


