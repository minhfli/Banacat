#include "Player.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <math.h>

#include <sk_engine/Common/sk_time.h>

#include <sk_engine/Window/Input.h>

#include <sk_engine/Graphics/Graphics.h>

#include "AllEntity.h"
#include "../Area.h"
#include "../Level.h"

struct Player::playerdata { // value only
    playerdata() {}
    ~playerdata() {}

    bool is_dead = false;

    glm::vec2 collider_size = { 1,1.5f };
    glm::vec2 spawn_point;
    // x: ad    y: ws
    glm::vec2 wasd_dir;
    float facing = 1;

    // acceleration amount, time to get to top speed = runspeed/accel
    float accel_after_wallbounce;
    float accel = 50;
    float runspeed = 6;

    bool is_grounded = false;
    bool is_touching_wall_left = false;
    bool is_touching_wall_right = false;

    // for jumping
    float gravity_normal = 50;
    float gravity_low = 30;
    float gravity_high = 180;
    float gravity_peak = 15; // gravity when player is in peak of the jump

    float gravity_in_transition;

    float last_space_donw_time = -1;

    bool is_wallbounce = false;
    bool is_jumping = false;
    float jump_maxheight = 3.5;
    float jump_maxduration = sqrt(2.0f * jump_maxheight / gravity_low);

    float jumpvelocity = sqrt(jump_maxheight * gravity_low * 2);
    float jumpvelocity_peak = 0.7f;
    float jump_start_time;

    bool in_gravity_transition = false;
    float gravity_transition_start_time;
    float gravity_transition_time;

    // for dashing
    bool candash = false;
    float dashspeed = 25;
    float dashtime = 0.2f;
    float last_dash_time = -100;
    bool is_dashing = false;
};
Player::Player() {
    pdata = new playerdata();
}
Player::~Player() {
    delete pdata;
}

namespace { // some helper function

    inline float move_to(const float a, const float b, const float step) {
        if (a >= b) {
            if (a - step >= b)
                return a - step;
            return b;
        }
        else {
            if (a + step <= b)
                return a + step;
            return b;
        }
    }

    // a: reference,    b: result
    inline float get_relative_lerp(float a, float a1, float a2, float b1, float b2) {
        return b1 + (a - a1) / (a2 - a1) * (b2 - b1);
    }
    inline float get_relative_lsin(float a, float a1, float a2, float b1, float b2) {
        return b1 + sin((a - a1) / (a2 - a1) * M_PI) * (b2 - b1);
    }
}
void Player::OnCreate(Area* area, Level* level) {
    m_area = area;
    m_level = level;

    physic_world = m_area->GetPhysicWorld();

    SetSpawnPoint(m_level->special_level_data.default_spawn_point);

    AddTag_(e_tag::PHY_ACTOR);
    AddTag_(e_tag::PHY_MOVEABLE);
    AddTag_(e_tag::PLAYER);
    sk_physic2d::Body_Def player_bodydef(
        sk_physic2d::irect::irect_fbound({ pdata->spawn_point, pdata->spawn_point + pdata->collider_size }),
        tag,
        this
    );
    m_body_index = physic_world->Create_Body(player_bodydef);
}
void Player::OnDestroy() {
    physic_world->Remove_Body(m_body_index);
}
void Player::Start() {

}
void Player::Update() {
    //std::cout << "player update called\n";
    //std::cout << "player body ptr: " << m_body << '\n';

    if (sk_input::KeyDown(sk_key::R)) pdata->is_dead = true;
    if (!pdata->is_dead)
        SolveMovement();
    else SolveDeath();
    SolveAnimation();

    sk_physic2d::Body* m_body = physic_world->Get_Body(m_body_index);
    sk_graphic::Renderer2D_GetCam()->focus.pos = m_body->RECT.true_center();
}
void Player::SolveMovement() {
    sk_physic2d::Body* m_body = physic_world->Get_Body(m_body_index);
    // setup input and collision checking
    if (!sk_input::Key(sk_key::A) && !sk_input::Key(sk_key::D)) pdata->wasd_dir.x = 0;
    if (sk_input::Key(sk_key::A) && !sk_input::Key(sk_key::D)) pdata->wasd_dir.x = -1;
    if (!sk_input::Key(sk_key::A) && sk_input::Key(sk_key::D)) pdata->wasd_dir.x = 1;
    if (sk_input::Key(sk_key::A) && sk_input::Key(sk_key::D) && pdata->wasd_dir.x == 0) pdata->wasd_dir.x = 1;

    if (!sk_input::Key(sk_key::W) && !sk_input::Key(sk_key::S)) pdata->wasd_dir.y = 0;
    if (sk_input::Key(sk_key::W) && !sk_input::Key(sk_key::S)) pdata->wasd_dir.y = 1;
    if (!sk_input::Key(sk_key::W) && sk_input::Key(sk_key::S)) pdata->wasd_dir.y = -1;
    if (sk_input::Key(sk_key::W) && sk_input::Key(sk_key::S) && pdata->wasd_dir.y == 0) pdata->wasd_dir.y = 1;

    if (pdata->wasd_dir.x != 0) pdata->facing = pdata->wasd_dir.x;

    //pdata->is_grounded = physic_world->TouchSolid_ibound(m_body->RECT.bound + glm::ivec4({ 0,-1,0,-1 }));
    pdata->is_grounded = physic_world->BoxCast(
        m_body->RECT.bound + glm::ivec4(0, -1, 0, -1),
        (1 << e_tag::PHY_SOLID) +
        (1 << e_tag::GROUND)
    );
    if (pdata->is_grounded == false)
        pdata->is_grounded = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(0, -1, 0, -1),
            (1 << e_tag::PHY_SOLID) +
            (1 << e_tag::PHY_ONE_WAY)
        ) && m_body->velocity.y == 0;
    pdata->is_touching_wall_left = physic_world->BoxCast(
    m_body->RECT.bound + glm::ivec4(-2, 0, -2, 0),
    (1 << e_tag::PHY_SOLID) +
    (1 << e_tag::GROUND)
    );
    pdata->is_touching_wall_right = physic_world->BoxCast(
        m_body->RECT.bound + glm::ivec4(2, 0, 2, 0),
        (1 << e_tag::PHY_SOLID) +
        (1 << e_tag::GROUND)
    );
    if (pdata->is_grounded) pdata->candash = 1;

    // solving
    if (pdata->is_dashing && sk_time::current_time > pdata->last_dash_time + pdata->dashtime) {
        pdata->is_dashing = false;
        m_body->velocity.x = std::clamp(m_body->velocity.x, -8.0f, 8.0f);
        m_body->velocity.y = std::clamp(m_body->velocity.y, -6.0f, 6.0f);
    }
    if (!pdata->is_dashing) { // dashing overwrite every other movement(run, jump, wallgrab)
        // running
        if (pdata->is_wallbounce) {
            pdata->accel_after_wallbounce = move_to(
                pdata->accel_after_wallbounce,
                pdata->accel,
                50.0f * sk_time::delta_time
            );
            m_body->velocity.x = move_to(
                    m_body->velocity.x,
                    pdata->runspeed * pdata->wasd_dir.x,
                    pdata->accel_after_wallbounce * sk_time::delta_time
            );
        }
        else m_body->velocity.x = move_to(
                    m_body->velocity.x,
                    pdata->runspeed * pdata->wasd_dir.x,
                    pdata->accel * sk_time::delta_time
        );

        //jumping
        if (pdata->is_jumping) {
            if (m_body->velocity.y <= pdata->jumpvelocity_peak) {
                pdata->is_jumping = false;
                pdata->is_wallbounce = false;
            }
            else if (!sk_input::Key(sk_key::SPACE)) {
                if (!pdata->in_gravity_transition) {
                    pdata->gravity_transition_start_time = sk_time::current_time;
                    pdata->in_gravity_transition = true;
                    pdata->gravity_transition_time = std::max(sk_time::current_time - pdata->jump_start_time, 0.05f);
                    pdata->gravity_in_transition = pdata->gravity_low;
                }
                float target_gravity = get_relative_lsin( // the higher the velocity, the higher the gravity
                    m_body->velocity.y,
                    0,
                    pdata->jumpvelocity,
                    pdata->gravity_normal,
                    pdata->gravity_high
                );
                pdata->gravity_in_transition = get_relative_lsin(
                    sk_time::current_time - pdata->gravity_transition_start_time,
                    0,
                    pdata->gravity_transition_time,

                    pdata->gravity_in_transition,
                    target_gravity
                );
                m_body->velocity.y -= pdata->gravity_in_transition * sk_time::delta_time;
                if (m_body->velocity.y < pdata->jumpvelocity_peak) {
                    float remain_time = (pdata->jumpvelocity_peak - m_body->velocity.y) / pdata->gravity_in_transition;
                    m_body->velocity.y = pdata->jumpvelocity_peak - pdata->gravity_peak * remain_time;
                }
            }
            else
                m_body->velocity.y -= pdata->gravity_low * sk_time::delta_time;

        }
        if (!pdata->is_jumping) {
            if (pdata->jumpvelocity_peak >= m_body->velocity.y && m_body->velocity.y >= -0.05f)
                m_body->velocity.y -= pdata->gravity_peak * sk_time::delta_time;
            else {
                m_body->velocity.y -= pdata->gravity_normal * sk_time::delta_time;
                //std::cout << m_body->velocity.y << " ";
            }
        }
        if (sk_input::KeyDown(sk_key::SPACE) && pdata->is_grounded) {
            pdata->is_jumping = true;
            pdata->is_wallbounce = false;
            pdata->in_gravity_transition = false;
            pdata->jump_start_time = sk_time::current_time;
            m_body->velocity.y = pdata->jumpvelocity;
        }
        if (sk_input::KeyDown(sk_key::SPACE) && !pdata->is_grounded && pdata->is_touching_wall_left) {
            pdata->is_jumping = true;
            pdata->is_wallbounce = true;
            pdata->in_gravity_transition = false;
            pdata->jump_start_time = sk_time::current_time;

            pdata->accel_after_wallbounce = 0;
            m_body->velocity.y = pdata->jumpvelocity;
            m_body->velocity.x = pdata->runspeed + 1;
        }
        if (sk_input::KeyDown(sk_key::SPACE) && !pdata->is_grounded && pdata->is_touching_wall_right) {
            pdata->is_jumping = true;
            pdata->is_wallbounce = true;
            pdata->in_gravity_transition = false;
            pdata->jump_start_time = sk_time::current_time;

            pdata->accel_after_wallbounce = 0;
            m_body->velocity.y = pdata->jumpvelocity;
            m_body->velocity.x = -pdata->runspeed - 1;
        }

        // dashing
        if (sk_input::KeyDown(sk_key::K) && pdata->candash) {
            pdata->candash = 0;
            pdata->last_dash_time = sk_time::current_time;
            pdata->is_dashing = true;
            pdata->is_jumping = false;

            glm::vec2 dash_dir = pdata->wasd_dir;
            if (dash_dir == glm::vec2(0)) dash_dir.x = pdata->facing;
            m_body->velocity = glm::normalize(dash_dir) * pdata->dashspeed;
        }
    }

}
void Player::SolveDeath() {
    pdata->is_dead = false;
    sk_physic2d::Body* m_body = physic_world->Get_Body(m_body_index);
    m_body->RECT = sk_physic2d::irect::irect_fbound(glm::vec4(pdata->spawn_point, pdata->spawn_point + pdata->collider_size));
}
void Player::SolveAnimation() {}

void Player::OnTrigger(Entity* trigger) {
    if (trigger->CheckTag_(e_tag::SPAWN_POINT)) {
        pdata->spawn_point = ((PlayerSpawn*)trigger)->spawn_point;
        return;
    }
    if (trigger->CheckTag_(e_tag::DASH_REFRESH)) {
        if (!pdata->candash && ((DashRefresh*)trigger)->is_active) {
            pdata->candash = true;
            ((DashRefresh*)trigger)->is_active = false;
            ((DashRefresh*)trigger)->last_triggered_time = sk_time::current_time;
        }
        return;
    }

}
void Player::OnTrigger(uint64_t trigger_tag) {
    if (CheckTag(trigger_tag, e_tag::DAMAGE)) {
        pdata->is_dead = true;
    }
}
void Player::Draw() {
    auto m_body = physic_world->Get_Body(m_body_index);
    sk_graphic::Renderer2D_AddBBox(m_body->RECT.true_bound(), 2, glm::vec4(1));

    sk_graphic::Renderer2D_AddDotX(glm::vec3(pdata->spawn_point, 4), { 0,0,1,1 });
}

void Player::SetSpawnPoint(glm::vec2 p) {
    pdata->spawn_point = p;
}
