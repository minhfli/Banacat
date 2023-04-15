#pragma once

#include "Entity.h"

#include <sk_engine/Physics/Collider.h>
#include <sk_engine/Physics/AABB.h>
#include <GLM/glm.hpp>

class Player:public Entity {
    public:
    Player();
    ~Player();

    private: // data

    struct playerdata;

    playerdata* pdata;

    public:
    sk_physic2d::Body* m_body;
    sk_physic2d::AABB_World* physic_world;

    void Start() override;
    void Update() override;
    void Draw() override;
};

