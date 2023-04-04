#pragma once

#include <sk_engine/Physics/Collider.h>
#include <GLM/glm.hpp>

class Player {
    public:
    Player() {}
    ~Player() {}

    glm::vec2 pos = glm::vec2(0);
    sk_physic2d::Body* m_body;

    void Update();
    void Draw();
};

