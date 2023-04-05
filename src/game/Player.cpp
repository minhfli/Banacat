#include "Player.h"

#include <sk_engine/Common/sk_time.h>

#include <sk_engine/Window/Input.h>

#include <sk_engine/Graphics/2D_Renderer.h>

void Player::Update() {

    m_body->velocity.x = 0;
    float speed = 5.0f;
    if (sk_input::Key(sk_key::A)) m_body->velocity.x = -speed;
    if (sk_input::Key(sk_key::D)) m_body->velocity.x = speed;
    if (sk_input::Key(sk_key::SPACE)) m_body->velocity.y = speed;

    m_body->velocity.y -= 20 * sk_time::delta_time;
    //m_body->RECT.pos = pos;
}
void Player::Draw() {
    sk_graphic::Renderer2D_AddBBox(m_body->RECT.bound(), 2, glm::vec4(1));

}