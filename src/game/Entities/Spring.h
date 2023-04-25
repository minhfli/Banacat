#pragma once
#include "../Entity.h"
#include <GLM/glm.hpp>

class Spring: public Entity {
    public:
    Spring(/* args */) {}
    ~Spring() {}

    int trigger_body = -1;
    glm::vec2 pos;

    void OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) override;
    void OnCreate(Area* area, Level* level) override {}
    void OnDestroy() override;

    void OnActive() override {}
    void OnDeActive() override {}

    void OnTrigger(Entity* trigger) override {}
    void OnTrigger(uint64_t trigger_tag) override {}

    void Start() override {}
    void Update() override {}
    void LateUpdate() override {}
    void Draw() override {}
};

