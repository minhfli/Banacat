#pragma once

#include <sk_engine/Physics/AABB.h>
#include "../Entity.h"
#include <GLM/glm.hpp>

class Area;
class Level;

enum class Scene;

class AreaFinish : public Entity {
    public:
    AreaFinish() {}
    ~AreaFinish() {}

    int trigger_body;
    Scene scene_to_load;

    void OnJsonCreate(Area* area, Level* level, nlohmann::json& jentity) override;
    void OnCreate(Area* area, Level* level) override {}
    void OnDestroy() override;

    void OnActive() override {}
    void OnDeActive() override {}

    void OnTrigger(Entity* trigger) override;
    void OnTrigger(uint64_t trigger_tag) override;

    void Start() override {}
    void Update() override {}
    void LateUpdate() override {}
    void Draw() override;
};


