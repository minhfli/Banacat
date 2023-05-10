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

#include "Animation.h"

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
    float get_relative_lerp(float a, float a1, float a2, float b1, float b2) {
        return b1 + (a - a1) / (a2 - a1) * (b2 - b1);
    }
    float get_relative_lsin(float a, float a1, float a2, float b1, float b2) {
        return b1 + sin((a - a1) / (a2 - a1) * M_PI * 0.5f) * (b2 - b1);
    }
    glm::vec2 get_relative_lsin(float a, float a1, float a2, glm::vec2 b1, glm::vec2 b2) {
        return b1 + (float)sin((a - a1) / (a2 - a1) * M_PI * 0.5f) * (b2 - b1);
    }

    glm::vec2 get_pivot_pos(glm::vec2 pivot, glm::vec4 bound) {
        return
            glm::vec2(bound.x, bound.y) * (glm::vec2(1) - pivot) +
            glm::vec2(bound.z, bound.w) * pivot;
    }
}
struct Player::playerdata { // value only
    playerdata() {}
    ~playerdata() {}

    sk_physic2d::Body* m_body;
    sk_physic2d::AABB_World* physic_world;

    glm::vec2 collider_size = { 1,2 };
    glm::vec2 spawn_point;

    bool is_dead = false;
    float death_time;
    glm::vec2 death_pos;
    glm::vec2 death_target_pos;
    float death_duration = 1.0f;

    // x: ad    y: ws
    glm::vec2 wasd_dir;
    float facing = 1;

    float terminal_velocity = -30.0f;

    // acceleration amount, time to get to top speed = runspeed/accel
    float accel_after_wallkick;
    float accel_after_springpush;
    float accel = 60;
    float runspeed = 7.2f;

    bool is_grounded = false;
    bool is_touching_wall_left = false;     // stand next to a wall
    bool is_touching_wall_right = false;    // stand next to a wall
    bool is_touching_wall_left_up = false;     // stand next to a wall
    bool is_touching_wall_right_up = false;    // stand next to a wall
    bool is_touching_wall_up = false;    // stand next to a wall

    bool is_near_wall_left_3px = false;     // stand near a wall 
    bool is_near_wall_right_3px = false;    // stand near a wall
    bool is_near_spike_left = false;     // near spike
    bool is_near_spike_right = false;    // near spike

    // for jumping
    float coyote_time = 0.1f;
    float last_grounded_time;

    float gravity_normal = 50;
    float gravity_low = 30;
    float gravity_high = 400;
    float gravity_peak = 20; // gravity when player is in peak of the jump

    float gravity_in_transition;

    float input_buffer_time = 0.15f;

    float DASH_input_time = -100;
    float JUMP_input_time = -100;

    bool is_wallkick = false; // jump away from wall
    bool is_walljump = false; // jump straight up when grabing wall
    bool is_jumping = false;
    bool is_solving_afterjump = false;
    float jump_maxheight = 3.2;
    float jump_maxduration = sqrt(2.0f * jump_maxheight / gravity_low);

    float jumpvelocity = sqrt(jump_maxheight * gravity_low * 2);
    float jumpvelocity_peak = 0.7f;
    float jump_start_time;

    bool in_gravity_transition = false;
    float gravity_transition_start_time;
    float gravity_transition_time;

    // for spring jump
    bool start_spring_jump = false;
    float spring_jump_velocity = 25;

    bool start_spring_push = false;
    bool is_spring_pushed = false;
    float spring_push_time = 0.4f;
    float spring_push_start_time;
    float spring_push_dir;
    glm::vec2 spring_push_velocity = { 23,10 };

    // for dashing
    bool candash = false;
    glm::vec2 dash_dir;
    float dashspeed = 25;
    float dashtime = 0.2f;
    float last_dash_time = -100;
    float dash_delay_time = 0.35f;
    float dash_reset_start_time = -100;
    float dash_reset_time = 0.1f;
    bool is_dashing = false;

    // for wall interaction
    bool is_wall_grabing = false;
    bool was_wall_grabing = false;
    float last_wallgrab_time;
    float wallgrab_dir = 0;
    float wall_climb_speed = 5.5;
    float wall_slide_terminal_velocity = -15.0f;
    float wall_slide_gravity = 25.0f;

    float wallgrab_max_stamina = 100;
    float wallgrab_tired_stamina_threshold = 20;
    float wallgrab_current_stamina;
    float wallgrab_hold_staminia = 20;
    float wallgrab_climb_staminia = 50;
    float wallgrab_jump_staminia = 25;

    void Update() {
        if (is_dead) Solve_Death();
        else Solve_Movement();
        Animation_SM();
    }
    void Draw() {
        if (is_dead) {
            death_pos.x = move_to(death_pos.x, death_target_pos.x, sk_time::delta_time * 10);
            death_pos.y = move_to(death_pos.y, death_target_pos.y, sk_time::delta_time * 10);
            anim_death.Draw(death_pos, 2, glm::vec2(0.5f, 0.333f));
        }
        else anim_main.Draw(
            get_pivot_pos({ 0.5,0 }, m_body->RECT.true_bound()) - glm::vec2(0, 1.0f / 8.0f), // lower center
            -1,
            glm::vec2(0.5f, 0)
        );
    }
    void Solve_Movement() {
        GetMovement_Input();
        Movement_PhysicCheck();
        // solving
        Movement_Run();
        Movement_Jump();
        Movement_Dash();
        Movement_WallGrab();
        Movement_Spring();
        Movement_Apply_Normal_Gravity();
    }
    void Solve_Death() {
        if (sk_time::current_time >= death_time + death_duration) {
            SetAlive();
            return;
        }
        anim_death.SetFrame_byCurrentTick();
    }
#pragma region Player death

    // direction to move player to when it dead
    void SetDead(glm::vec2 pushback_dir = glm::vec2(0)) {
        anim_death.SetState("death");
        if (candash) anim_death.SetLayer("yellow");
        else anim_death.SetLayer("green");

        // stop all movement
        is_jumping = false;
        is_dashing = false;
        is_solving_afterjump = false;
        is_spring_pushed = false;
        is_wall_grabing = false;

        is_dead = true;
        death_time = sk_time::current_time;
        death_pos = get_pivot_pos(glm::vec2(0.5f, 0), m_body->RECT.true_bound());
        death_target_pos = death_pos + glm::vec2(1, 2) * pushback_dir;

        //disable physic body
        m_body->is_active = false;
    }
    void SetAlive() {
        m_body->RECT = sk_physic2d::irect::irect_fbound(glm::vec4(spawn_point, spawn_point + collider_size));
        m_body->velocity = glm::vec2(0);
        m_body->is_active = true;
        is_dead = false;
    }
#pragma region Movement code
    void GetMovement_Input() {
        if (!sk_input::Key(sk_key::A) && !sk_input::Key(sk_key::D)) wasd_dir.x = 0;
        if (sk_input::Key(sk_key::A) && !sk_input::Key(sk_key::D)) wasd_dir.x = -1;
        if (!sk_input::Key(sk_key::A) && sk_input::Key(sk_key::D)) wasd_dir.x = 1;
        if (sk_input::Key(sk_key::A) && sk_input::Key(sk_key::D)) {
            if (sk_input::KeyDown(sk_key::A)) wasd_dir.x = -1;
            if (sk_input::KeyDown(sk_key::D)) wasd_dir.x = 1;
            if (wasd_dir.x == 0) wasd_dir.x = 1;
        }

        if (!sk_input::Key(sk_key::W) && !sk_input::Key(sk_key::S)) wasd_dir.y = 0;
        if (sk_input::Key(sk_key::W) && !sk_input::Key(sk_key::S)) wasd_dir.y = 1;
        if (!sk_input::Key(sk_key::W) && sk_input::Key(sk_key::S)) wasd_dir.y = -1;
        if (sk_input::Key(sk_key::W) && sk_input::Key(sk_key::S)) {
            if (sk_input::KeyDown(sk_key::W)) wasd_dir.y = 1;
            if (sk_input::KeyDown(sk_key::S)) wasd_dir.y = -1;
            if (wasd_dir.y == 0) wasd_dir.y = 1;
        }

        if (!is_wall_grabing && wasd_dir.x != 0) facing = wasd_dir.x;
    }
    void Movement_PhysicCheck() {
        is_grounded = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(0, -1, 0, -1),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        if (is_grounded == false)
            is_grounded = physic_world->BoxCast(
                m_body->RECT.bound + glm::ivec4(0, -1, 0, -1),
                (1 << etag::PHY_SOLID) +
                (1 << etag::PHY_ONE_WAY)
            ) && m_body->velocity.y == 0;

        is_touching_wall_up = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(0, 1, 0, 1),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        is_touching_wall_left = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(-1, 8, -1, -16),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        is_touching_wall_left_up = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(-1, 12, -1, -16),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        is_touching_wall_right = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(1, 8, 1, -16),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        is_touching_wall_right_up = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(1, 12, 1, -16),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        is_near_wall_left_3px = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(-3, 0, -3, 0),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        is_near_wall_right_3px = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(3, 0, 3, 0),
            (1 << etag::PHY_SOLID) +
            (1 << etag::GROUND)
        );
        is_near_spike_left = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(-3, 8, -3, 8),
            (1 << etag::DAMAGE)
        );
        is_near_spike_right = physic_world->BoxCast(
            m_body->RECT.bound + glm::ivec4(3, 8, 3, 8),
            (1 << etag::DAMAGE)
        );

    }

    void Movement_ResetDash() {
        //if (!is_dashing)
            //last_dash_time = -100;
        if (!candash) {
            candash = true;
            dash_reset_start_time = sk_time::current_time;
        }
    }
    void Movement_ResetStamina() {
        wallgrab_current_stamina = wallgrab_max_stamina;
    }

    void Movement_Run() {
        if (is_dashing) return;
        if (is_wall_grabing) return;
        if (is_wallkick) {
            accel_after_wallkick = move_to(
                accel_after_wallkick,
                accel,
                50.0f * sk_time::delta_time
            );
            m_body->velocity.x = move_to(
                    m_body->velocity.x,
                    runspeed * wasd_dir.x,
                    accel_after_wallkick * sk_time::delta_time
            );
            //if (m_body->velocity.x < 0) std::cout << "error\n";
        }
        else if (is_spring_pushed) {
            if (wasd_dir.x == -spring_push_dir)
                accel_after_springpush = move_to(
                    accel_after_springpush,
                    accel,
                    160.0f * sk_time::delta_time
                );
            if (wasd_dir.x == spring_push_dir)
                accel_after_springpush = move_to(
                    accel_after_springpush,
                    accel,
                    30.0f * sk_time::delta_time
                );
            if (wasd_dir.x == 0)
                accel_after_springpush = move_to(
                    accel_after_springpush,
                    accel,
                    80.0f * sk_time::delta_time
                );
            m_body->velocity.x = move_to(
                    m_body->velocity.x,
                    runspeed * wasd_dir.x,
                    accel_after_springpush * sk_time::delta_time
            );
        }
        else m_body->velocity.x = move_to(
                m_body->velocity.x,
                runspeed * wasd_dir.x,
                accel * sk_time::delta_time
        );
    }
    void Movement_Jump() {
        if (is_dashing) return;
        if (sk_input::KeyDown(sk_key::SPACE)) JUMP_input_time = sk_time::current_time;
        if (is_grounded && !is_jumping) last_grounded_time = sk_time::current_time;
        if (JUMP_input_time + input_buffer_time >= sk_time::current_time && last_grounded_time + coyote_time > sk_time::current_time) { // normal jump
            JUMP_input_time = -100;
            is_jumping = true;
            is_wallkick = false;
            is_walljump = false;
            in_gravity_transition = false;
            is_solving_afterjump = false;
            jump_start_time = sk_time::current_time;
            m_body->velocity.y = jumpvelocity;
        }
        if (JUMP_input_time + input_buffer_time >= sk_time::current_time && last_grounded_time + coyote_time <= sk_time::current_time) { // wall jump/kick
            if (is_near_wall_left_3px && is_near_wall_right_3px) { // touch both wall, do a normal jump
                JUMP_input_time = -100;
                is_jumping = true;
                is_wallkick = false;
                is_walljump = false;
                in_gravity_transition = false;
                is_solving_afterjump = false;
                m_body->velocity.y = jumpvelocity;
                jump_start_time = sk_time::current_time;
            }
            // if player want to do a walljump but not enough stamina, player will do a wallkick
            else if (is_near_wall_left_3px) {
                jump_start_time = sk_time::current_time;
                JUMP_input_time = -100;
                in_gravity_transition = false;
                is_solving_afterjump = false;
                is_jumping = true;
                m_body->velocity.y = jumpvelocity;

                if (is_wall_grabing && wasd_dir.x <= 0 && wallgrab_current_stamina >= wallgrab_jump_staminia) { // wall jump
                    wallgrab_current_stamina -= wallgrab_jump_staminia;
                    is_wallkick = false;
                    is_walljump = true;
                }
                else { // wall kick
                    is_wallkick = true;
                    is_walljump = false;
                    accel_after_wallkick = 0;
                    m_body->velocity.x = runspeed + 1;
                }
            }
            else if (is_near_wall_right_3px) {
                jump_start_time = sk_time::current_time;
                JUMP_input_time = -100;
                in_gravity_transition = false;
                is_solving_afterjump = false;
                is_jumping = true;
                m_body->velocity.y = jumpvelocity;

                if (is_wall_grabing && wasd_dir.x >= 0 && wallgrab_current_stamina >= wallgrab_jump_staminia) { // wall jump
                    wallgrab_current_stamina -= wallgrab_jump_staminia;
                    is_wallkick = false;
                    is_walljump = true;
                }
                else { // wall kick
                    is_wallkick = true;
                    is_walljump = false;
                    accel_after_wallkick = 0;
                    m_body->velocity.x = -runspeed - 1;
                    std::cout << "wall kick " << m_body->velocity.x << '\n';
                }
            }
        }
        if (is_jumping) {
            if (is_walljump && sk_time::current_time - jump_start_time <= 0.05f) { // player do a walljump but move away from wall
                if (is_touching_wall_left && wasd_dir.x == 1) {
                    wallgrab_current_stamina += wallgrab_jump_staminia;
                    is_walljump = false;
                    is_wallkick = true;
                    m_body->velocity.x = runspeed + 1;
                    accel_after_wallkick = 0;
                    std::cout << "wall jump to kick \n";
                }
                if (is_touching_wall_right && wasd_dir.x == -1) {
                    wallgrab_current_stamina += wallgrab_jump_staminia;
                    is_walljump = false;
                    is_wallkick = true;
                    m_body->velocity.x = runspeed + 1;
                    accel_after_wallkick = 0;
                    std::cout << "wall jump to kick \n";
                }

            }
            if (m_body->velocity.y <= jumpvelocity_peak) { // stop jumping
                is_jumping = false;
                is_wallkick = false;
                is_walljump = false;
            }
            else if (!sk_input::Key(sk_key::SPACE)) {
                is_jumping = false;
                is_solving_afterjump = true;
            }
            else
                m_body->velocity.y -= gravity_low * sk_time::delta_time;
        }
        else if (is_solving_afterjump) { // after player release jump key
            if (m_body->velocity.y <= jumpvelocity_peak) { // stop jumping
                is_jumping = false;
                is_solving_afterjump = false;
                is_wallkick = false;
                is_walljump = false;
            }
            else {
                if (!in_gravity_transition) {
                    gravity_transition_start_time = sk_time::current_time;
                    in_gravity_transition = true;
                    gravity_transition_time = std::max(sk_time::current_time - jump_start_time, 0.05f);
                    gravity_in_transition = gravity_low;
                }
                float target_gravity = get_relative_lsin( // the higher the velocity, the higher the gravity
                    m_body->velocity.y,
                    0,
                    jumpvelocity,
                    gravity_normal,
                    gravity_high
                );
                gravity_in_transition = get_relative_lsin(
                    sk_time::current_time - gravity_transition_start_time,
                    0,
                    gravity_transition_time,
                    gravity_in_transition,
                    target_gravity
                );
                m_body->velocity.y -= gravity_in_transition * sk_time::delta_time;
                if (m_body->velocity.y < jumpvelocity_peak) {
                    float remain_time = (jumpvelocity_peak - m_body->velocity.y) / gravity_in_transition;
                    m_body->velocity.y = jumpvelocity_peak - gravity_peak * remain_time;
                }
            }
        }

        if (!is_jumping && !is_solving_afterjump) is_wallkick = is_walljump = false;

    }
    void Movement_Dash() {
        if (is_grounded) Movement_ResetDash();
        if (is_dashing && sk_time::current_time > last_dash_time + dashtime) {
            is_dashing = false;
            m_body->velocity.x = std::clamp(m_body->velocity.x, -8.0f, 8.0f);
            m_body->velocity.y = std::clamp(m_body->velocity.y, -15.0f, 4.0f);
        }
        if (sk_input::KeyDown(sk_key::K)) DASH_input_time = sk_time::current_time;
        if (DASH_input_time + input_buffer_time >= sk_time::current_time &&
            candash &&
            !is_dashing &&
            sk_time::current_time >= last_dash_time + dash_delay_time &&
            !start_spring_jump &&
            !start_spring_push
            ) {
            candash = 0;
            last_dash_time = sk_time::current_time;
            DASH_input_time = -100;
            is_dashing = true;
            is_jumping = false;
            is_solving_afterjump = false;
            is_spring_pushed = false;

            dash_dir = wasd_dir;
            if (dash_dir == glm::vec2(0)) dash_dir.x = facing;
            m_body->velocity = glm::normalize(dash_dir) * dashspeed;
        }
    }
    void Movement_WallGrab() {
        if (is_grounded) wallgrab_current_stamina = wallgrab_max_stamina;
        is_wall_grabing =
            !is_jumping && !is_solving_afterjump && !is_dashing && sk_input::Key(sk_key::J) &&
            //m_body->velocity.y <= wall_climb_speed &&
            ((is_touching_wall_left && facing == -1) ||
            (is_touching_wall_right && facing == 1));

        //wallgrab_dir = 0;
        if (is_wall_grabing) {
            last_wallgrab_time = sk_time::current_time;
            wallgrab_dir = facing;
            if (wasd_dir.y == 0 && wallgrab_current_stamina > 0) { // hold
                wallgrab_current_stamina -= wallgrab_hold_staminia * sk_time::delta_time;
                m_body->velocity.y -= wall_slide_gravity * 3 * sk_time::delta_time;
                m_body->velocity.y = std::max(m_body->velocity.y, 0.0f);
            }
            if (wasd_dir.y == 1 && wallgrab_current_stamina > 0) { // climb
                if (is_touching_wall_up) {
                    wallgrab_current_stamina -= wallgrab_hold_staminia * sk_time::delta_time;
                    m_body->velocity.y = 0.5f;
                }
                else if ((wallgrab_dir == 1 && is_touching_wall_right_up) || (wallgrab_dir == -1 && is_touching_wall_left_up)) {
                    wallgrab_current_stamina -= wallgrab_climb_staminia * sk_time::delta_time;
                    m_body->velocity.y = wall_climb_speed;
                }
                else if ((wallgrab_dir == 1 && !is_near_spike_right) || (wallgrab_dir == -1 && !is_near_spike_left)) {
                    wallgrab_current_stamina -= wallgrab_climb_staminia * sk_time::delta_time;
                    m_body->velocity.y = wall_climb_speed;
                }
                else {
                    wallgrab_current_stamina -= wallgrab_hold_staminia * sk_time::delta_time;
                    m_body->velocity.y = 0;
                }
            }
            if (wasd_dir.y == -1 || wallgrab_current_stamina <= 0) { // slide
                m_body->velocity.y -= wall_slide_gravity * sk_time::delta_time;
                m_body->velocity.y = std::max(m_body->velocity.y, wall_slide_terminal_velocity);
            }
        }
        was_wall_grabing = (!is_wall_grabing && last_wallgrab_time + 0.15f > sk_time::current_time && sk_input::Key(sk_key::J) &&
            !is_dashing && !is_jumping && !is_solving_afterjump && !is_grounded &&
            !is_spring_pushed &&
            m_body->velocity.y < 6.0f && m_body->velocity.y >= 0 &&
            wasd_dir.y >= 0 &&
            (wasd_dir.x == wallgrab_dir || wasd_dir.x == 0)
        );
        if (was_wall_grabing) {
            // push player up the edge
            if (wallgrab_dir == -1 && !is_near_spike_left) m_body->velocity = { wallgrab_dir * 8.0f,6 };
            if (wallgrab_dir == 1 && !is_near_spike_right) m_body->velocity = { wallgrab_dir * 8.0f,6 };
        }

    }
    void Movement_Apply_Normal_Gravity() {
        if (!is_jumping && !is_dashing && !is_wall_grabing && !is_solving_afterjump)
            if (jumpvelocity_peak >= m_body->velocity.y && m_body->velocity.y >= -0.05f)
                m_body->velocity.y -= gravity_peak * sk_time::delta_time;
            else {
                m_body->velocity.y -= gravity_normal * sk_time::delta_time;
                //std::cout << m_body->velocity.y << " ";
            }
        if (m_body->velocity.y < terminal_velocity) m_body->velocity.y = terminal_velocity;
    }

    void Movement_Spring() {
        if (start_spring_jump) {
            start_spring_jump = false;
            is_jumping = false;
            is_dashing = false;
            is_solving_afterjump = false;
            is_wall_grabing = false;
            m_body->velocity.y = spring_jump_velocity;
        }
        if (start_spring_push) {
            start_spring_push = false;
            is_spring_pushed = true;
            spring_push_start_time = sk_time::current_time;
            is_jumping = false;
            is_dashing = false;
            is_solving_afterjump = false;
            is_wall_grabing = false;
            m_body->velocity = spring_push_velocity * glm::vec2(spring_push_dir, 1);
            accel_after_springpush = 0;
        }
        if (is_spring_pushed) if (sk_time::current_time > spring_push_start_time + spring_push_time) is_spring_pushed = false;
    }
#pragma endregion
#pragma region Animation code
    // all animation is resolve with tick
    enum Ani_State {
        NONE,
        IDLE,
        RUN,
        DASH,
        JUMP,
        FALL,
        WALL_GRAB,
        WALL_CLIMB,
        WALL_SLIDE,
    } current_state = NONE;

    Animation anim_main;
    Animation anim_death;
    void SetupAnimation() {
        anim_main.Init("Assets/Entity/Player/bananacat", true);
        anim_death.Init("Assets/Entity/Player/death", true);
    }
    void Animation_SM() { // state machine
        Ani_State newstate = IDLE;
        if (is_wall_grabing) {
            newstate = WALL_GRAB;
            if (m_body->velocity.y > 1)  newstate = WALL_CLIMB;
            if (m_body->velocity.y < -1) newstate = WALL_SLIDE;
        }
        else if (is_dashing) newstate = DASH;
        else if (!is_grounded) newstate = JUMP;
        else if (wasd_dir.x != 0) newstate = RUN;
        else newstate = IDLE;

        anim_main.flipx = (facing == -1);

        if (candash)
            if (dash_reset_start_time + dash_reset_time >= sk_time::current_time)
                anim_main.SetLayer("white");
            else
                anim_main.SetLayer("yellow");
        else
            anim_main.SetLayer("green");

        if (current_state != newstate) {
            current_state = newstate;

            switch (current_state) {
                case IDLE:       anim_main.SetState("idle");         break;
                case RUN:        anim_main.SetState("run");          break;
                case JUMP:       anim_main.SetState("jump");         break;
                case DASH:       anim_main.SetState("dash");         break;
                case WALL_GRAB:  anim_main.SetState("wall_grab");    break;
                case WALL_CLIMB: anim_main.SetState("wall_climb");    break;
                case WALL_SLIDE: anim_main.SetState("wall_slide");    break;

                default: break;
            }

        }
        switch (current_state) {
            case IDLE:          Animation_Idle();       break;
            case RUN:           Animation_Run();        break;
            case JUMP:          Animation_Jump();       break;
            case DASH:          Animation_Dash();       break;
            case WALL_GRAB:     Animation_WallGrab();   break;
            case WALL_CLIMB:    Animation_WallGrab();   break;
            case WALL_SLIDE:    Animation_WallGrab();   break;

            default: break;
        }
    }
    void Animation_Idle() {
        anim_main.SetFrame_byCurrentTick();
    }
    void Animation_Run() {
        anim_main.SetFrame_byCurrentTick();
    }
    void Animation_Jump() {
        if (m_body->velocity.y >= 15) anim_main.SetFrame_byIndex(0);
        else if (m_body->velocity.y >= 10) anim_main.SetFrame_byIndex(1);
        else if (m_body->velocity.y >= 5) anim_main.SetFrame_byIndex(2);
        else if (m_body->velocity.y >= 0) anim_main.SetFrame_byIndex(3);
        else if (m_body->velocity.y >= -4) anim_main.SetFrame_byIndex(4);
        else if (m_body->velocity.y >= -10) anim_main.SetFrame_byIndex(5);
        else anim_main.SetFrame_byIndex(6);
    }
    void Animation_Dash() {
        if (dash_dir.y == 0) anim_main.SetFrame_byIndex(0);     // full horizontal dash
        if (dash_dir.y == 1) anim_main.SetFrame_byIndex(1);     // upward dash
        if (dash_dir.y == -1) anim_main.SetFrame_byIndex(2);    // downward dash
    }
    void Animation_WallGrab() {
        if (current_state == WALL_GRAB)
            anim_main.SetFrame_byIndex(0);
        if (current_state == WALL_CLIMB)
            anim_main.SetFrame_byCurrentTick();
        if (current_state == WALL_SLIDE)
            anim_main.SetFrame_byCurrentTick();
    }
#pragma endregion
};
Player::Player() {
    pdata = new playerdata();
}
Player::~Player() {
    delete pdata;
}
void Player::OnNewLevel() {

    pdata->Movement_ResetDash();
    pdata->Movement_ResetStamina();

    m_level = m_area->Active_level;
    pdata->spawn_point = m_level->special_level_data.default_spawn_point;
}

void Player::OnCreate(Area* area, Level* level) {
    m_area = area;
    m_level = level;

    physic_world = m_area->GetPhysicWorld();
    pdata->physic_world = physic_world;

    SetSpawnPoint(m_level->special_level_data.default_spawn_point);

    AddTag_(etag::PHY_ACTOR);
    AddTag_(etag::PHY_MOVEABLE);
    AddTag_(etag::PLAYER);
    sk_physic2d::Body_Def player_bodydef(
        sk_physic2d::irect::irect_fbound({ pdata->spawn_point, pdata->spawn_point + pdata->collider_size }),
        tag,
        this
    );
    m_body_index = physic_world->Create_Body(player_bodydef);
    pdata->m_body = physic_world->Get_Body(m_body_index);

    pdata->SetupAnimation();
}
void Player::OnDestroy() {
    physic_world->Remove_Body(m_body_index);
}
void Player::Start() {
    sk_graphic::Renderer2D_GetCam()->position = glm::vec3(GetCameraTarget(), 0);
    sk_graphic::Renderer2D_GetCam()->focus.pos = GetCameraTarget();
}
void Player::Update() {
    //std::cout << "player update called\n";
    //std::cout << "player body ptr: " << m_body << '\n';

    if (sk_input::KeyDown(sk_key::R)) pdata->SetDead();
    pdata->Update();

    sk_graphic::Renderer2D_GetCam()->focus.pos = GetCameraTarget();
}
void Player::LateUpdate() {

}
void Player::OnTrigger(Entity* trigger) {
    if (trigger->CheckTag_(etag::SPAWN_POINT)) {
        pdata->spawn_point = ((PlayerSpawn*)trigger)->spawn_point;
        return;
    }
    if (trigger->CheckTag_(etag::DASH_CRYSTAL)) {
        if (!pdata->candash && ((DashCrystal*)trigger)->is_active) {
            pdata->Movement_ResetDash();
            pdata->Movement_ResetStamina();
            ((DashCrystal*)trigger)->OnPlayerInteract();
        }
        return;
    }
    if (trigger->CheckTag_(etag::SPRING)) {
        pdata->Movement_ResetDash();
        pdata->Movement_ResetStamina();
        if (trigger->CheckTag_(etag::DIR_U))
            pdata->start_spring_jump = true;
        if (trigger->CheckTag_(etag::DIR_L)) {
            pdata->start_spring_push = true;
            pdata->spring_push_dir = -1;
        }
        if (trigger->CheckTag_(etag::DIR_R)) {
            pdata->start_spring_push = true;
            pdata->spring_push_dir = 1;
        }
        return;
    }
}
void Player::OnTrigger(uint64_t trigger_tag) {
    if (CheckTag(trigger_tag, etag::DAMAGE)) {
        if (CheckTag(trigger_tag, etag::PHY_DIR_U)) pdata->SetDead({ -pdata->facing, 1 });
        else if (CheckTag(trigger_tag, etag::PHY_DIR_D)) pdata->SetDead({ -pdata->facing, -1 });
        else if (CheckTag(trigger_tag, etag::PHY_DIR_L)) pdata->SetDead({ -2.5f, 0 });
        else if (CheckTag(trigger_tag, etag::PHY_DIR_R)) pdata->SetDead({ 2.5f, 0 });
        else pdata->SetDead({ -pdata->facing, 1 });
    }
}
void Player::Draw() {

    sk_graphic::Renderer2D_AddDotX(glm::vec3(pdata->spawn_point, 4), { 0,0,1,1 });
    pdata->Draw();
}

void Player::SetSpawnPoint(glm::vec2 p) {
    pdata->spawn_point = p;
}

glm::vec2 Player::GetCameraTarget() {
    return physic_world->Get_Body(m_body_index)->RECT.true_center();
}