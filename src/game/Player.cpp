#include "Player.h"

#include <sk_engine/Common/sk_time.h>

#include <sk_engine/Window/Input.h>

#include <sk_engine/Graphics/2D_Renderer.h>

void Player::Update() {

    float speed = 5.0f;
    if (sk_input::Key(sk_key::A)) m_body->velocity.x = -speed;
    if (sk_input::Key(sk_key::D)) m_body->velocity.x = speed;
    if (sk_input::Key(sk_key::W)) m_body->velocity.y = speed;
    if (sk_input::Key(sk_key::S)) m_body->velocity.y = -speed;
    //m_body->RECT.pos = pos;
}
void Player::Draw() {
    sk_graphic::Renderer2D_AddBBox(m_body->RECT.bound(), 2, glm::vec4(1));

}