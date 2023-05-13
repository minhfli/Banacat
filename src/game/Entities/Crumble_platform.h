#pragma once

#include "../Entity.h"
#include "Animation.h"

class Crumble_Platform :public Entity {
    public:
    Crumble_Platform();
    ~Crumble_Platform();

    private:
    struct PRIVATE_DATA;
    PRIVATE_DATA* pdata;

    public:

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

    void OnRiding(Entity* entity) override;
};


