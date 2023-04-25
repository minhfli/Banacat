#pragma once

#include "../Entity.h"

#include <sk_engine/Graphics/Graphic2D.h>
#include <sk_engine/Physics/Collider.h>
#include <sk_engine/Physics/AABB.h>
#include <GLM/glm.hpp>

#include "Animation.h"

class Player:public Entity {
    public:
    Player();
    ~Player();

    private: // data

    struct playerdata;

    playerdata* pdata;

    public:
    int m_body_index;
    sk_physic2d::AABB_World* physic_world;

    sk_graphic::Sprite2D sprite;

    Animation ani;

    // call when create player

    // level is default level, all level should be inititalized before call player
    void OnCreate(Area* area, Level* level) override;
    void OnDestroy() override;

    // call when new level is active, after transition
    void OnActive() override {}
    // call when new level is active, before transition
    void OnDeActive() override {}

    void Start() override;
    void Update() override;
    void Draw() override;

    void OnTrigger(Entity* trigger) override;
    void OnTrigger(uint64_t trigger_tag) override;

    void SetSpawnPoint(glm::vec2 p);

    glm::vec2 GetCameraTarget();
    private:
    void SolveMovement();
    void SolveAnimation();
    void SolveDeath();

};

