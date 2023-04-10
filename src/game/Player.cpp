#include "Player.h"

#include <iostream>
#include <algorithm>

#include <sk_engine/Common/sk_time.h>

#include <sk_engine/Window/Input.h>

#include <sk_engine/Graphics/Graphics.h>

struct Player::playerdata { // value only
    playerdata() {}
    ~playerdata() {}

    // x: ad    y: ws
    glm::vec2 wasd_dir;
    float facing = 1;

    // acceleration amount, time to get to top speed = runspeed/accel
    float accel = 50;
    float runspeed = 6;

    bool is_grounded = false;

    // for jumping
    float gravity_normal = 50;
    float gravity_low = 40;
    float gravity_high = 180;

    float gravity_in_transition;

    bool is_jumping = false;
    float jump_maxheight = 3.5;
    float jump_maxduration = sqrt(2.0f * jump_maxheight / gravity_low);

    float jumpvelocity = sqrt(jump_maxheight * gravity_low * 2);
    float jumpvelocity_peak = 0.1f; // if y speed is smaller than this, apply normal gravity
    float jump_start_time;

    bool in_gravity_transition = false;
    float gravity_transition_start_time;
    float gravity_transition_time;

    // for dashing
    bool candash = false;
    float dashspeed = 25;
    float dashtime = 0.17f;
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
}

void Player::Start() {
    std::cout << "Jump velocity: " << pdata->jumpvelocity << '\n';
}
void Player::Update() {

    // setup
    if (!sk_input::Key(sk_key::A) && !sk_input::Key(sk_key::D)) pdata->wasd_dir.x = 0;
    if (sk_input::Key(sk_key::A) && !sk_input::Key(sk_key::D)) pdata->wasd_dir.x = -1;
    if (!sk_input::Key(sk_key::A) && sk_input::Key(sk_key::D)) pdata->wasd_dir.x = 1;
    if (sk_input::Key(sk_key::A) && sk_input::Key(sk_key::D) && pdata->wasd_dir.x == 0) pdata->wasd_dir.x = 1;

    if (!sk_input::Key(sk_key::W) && !sk_input::Key(sk_key::S)) pdata->wasd_dir.y = 0;
    if (sk_input::Key(sk_key::W) && !sk_input::Key(sk_key::S)) pdata->wasd_dir.y = 1;
    if (!sk_input::Key(sk_key::W) && sk_input::Key(sk_key::S)) pdata->wasd_dir.y = -1;
    if (sk_input::Key(sk_key::W) && sk_input::Key(sk_key::S) && pdata->wasd_dir.y == 0) pdata->wasd_dir.y = 1;

    if (pdata->wasd_dir.x != 0) pdata->facing = pdata->wasd_dir.x;

    pdata->is_grounded = physic_world->TouchSolid_ibound(m_body->RECT.bound + glm::ivec4({ 0,-1,0,-1 }));
    if (pdata->is_grounded) pdata->candash = 1;

    // solving
    if (pdata->is_dashing && sk_time::current_time > pdata->last_dash_time + pdata->dashtime) {
        pdata->is_dashing = false;
        m_body->velocity.x = std::clamp(m_body->velocity.x, -8.0f, 8.0f);
        m_body->velocity.y = std::clamp(m_body->velocity.y, -6.0f, 6.0f);
    }
    if (!pdata->is_dashing) { // dashing overwrite every other movement(run, jump, wallgrab)
        // running
        m_body->velocity.x = move_to(
            m_body->velocity.x,
            pdata->runspeed * pdata->wasd_dir.x,
            pdata->accel * sk_time::delta_time
        );

        //jumping
        if (pdata->is_jumping) {
            if (
                m_body->velocity.y < pdata->jumpvelocity_peak ||
                sk_time::current_time >= pdata->jump_start_time + pdata->jump_maxduration
                ) {
                pdata->is_jumping = false;
            }
            else if (!sk_input::Key(sk_key::SPACE)) {
                if (!pdata->in_gravity_transition) {
                    pdata->gravity_transition_start_time = sk_time::current_time;
                    pdata->in_gravity_transition = true;

                    pdata->gravity_transition_time = std::max(sk_time::current_time - pdata->jump_start_time, 0.05f);

                    pdata->gravity_in_transition = pdata->gravity_low;
                }

                float target_gravity = get_relative_lerp( // the higher the velocity, the higher the gravity
                    m_body->velocity.y,
                    0,
                    pdata->jumpvelocity,
                    pdata->gravity_normal,
                    pdata->gravity_high
                );
                pdata->gravity_in_transition = get_relative_lerp(
                    sk_time::current_time - pdata->gravity_transition_start_time,
                    0,
                    pdata->gravity_transition_time,

                    pdata->gravity_in_transition,
                    target_gravity
                );
                m_body->velocity.y -= pdata->gravity_in_transition * sk_time::delta_time;
                if (m_body->velocity.y < 0) m_body->velocity.y = 0;
                //std::cout << "in gravity transition\n";
                //std::cout << "current gravity: " << pdata->gravity_in_transition << '\n';
                //std::cout << "targert gravity: " << target_gravity << '\n';
            }
            else
                m_body->velocity.y -= pdata->gravity_low * sk_time::delta_time;

        }
        if (!pdata->is_jumping)
            m_body->velocity.y -= pdata->gravity_normal * sk_time::delta_time;
        if (sk_input::KeyDown(sk_key::SPACE) && pdata->is_grounded && !pdata->is_jumping) {
            pdata->is_jumping = true;
            pdata->in_gravity_transition = false;
            pdata->jump_start_time = sk_time::current_time;
            m_body->velocity.y = pdata->jumpvelocity;
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

    sk_graphic::Renderer2D_GetCam()->focus.pos = m_body->RECT.true_center();
}
void Player::Draw() {
    sk_graphic::Renderer2D_AddBBox(m_body->RECT.true_bound(), 2, glm::vec4(1));
}