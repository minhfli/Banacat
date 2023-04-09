#pragma once

#include <sk_engine/Physics/Collider.h>
#include <sk_engine/Physics/AABB.h>
#include <GLM/glm.hpp>

class Player {
    public:
    Player() {}
    ~Player() {}

    sk_physic2d::Body* m_body;
    sk_physic2d::AABB_World* physic_world;

    void Update();
    void Draw();
};

