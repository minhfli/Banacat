#include "Test.h"
#include <Graphics/2D_Renderer.h>

namespace sk_physic2d {

    rect sbox(glm::vec2(-1), glm::vec2(6, 2));
    rect dbox(glm::vec2(-3), glm::vec2(1, 1));
    contact contact_data;
    contact contact_data_overlap;
    void Setup() {
        swept_rect_vs_rect(dbox, sbox, &contact_data, 1);
        colli_rect_vs_rect(dbox, sbox, &contact_data_overlap);
    }
    void Update(uint32_t delta_time, Camera& cam) {
        glm::vec3 mouse_pos = cam.Screen_To_World(sk_input::MousePos(), glm::vec2(800, 600));
        dbox.velocity.x = mouse_pos.x - dbox.center().x;
        dbox.velocity.y = mouse_pos.y - dbox.center().y;

        if (sk_input::Key(sk_key::SPACE)) {
            dbox.set_center(glm::vec2(mouse_pos.x, mouse_pos.y));
        }
        swept_rect_vs_rect(dbox, sbox, &contact_data, 1);
        colli_rect_vs_rect(dbox, sbox, &contact_data_overlap);

    }
    void Draw() {
        sk_graphic::Renderer2D_AddLine(glm::vec3(dbox.center(), 1), dbox.velocity, glm::vec4(1, 1, 0, 1));

        sk_graphic::Renderer2D_AddLBox(glm::vec3(dbox.pos, 0), dbox.size, glm::vec4(0, 1, 0, 1));
        sk_graphic::Renderer2D_AddLBox(glm::vec3(sbox.pos, 0), sbox.size, glm::vec4(0, 1, 0, 1));
        if (contact_data.hit) {
            sk_graphic::Renderer2D_AddDotX(glm::vec3(contact_data.pos, 2));
            sk_graphic::Renderer2D_AddLBox(
                glm::vec3(contact_data.pos - dbox.size / 2.0f, 0),
                dbox.size,
                glm::vec4(0, 1, 0, 1));
            sk_graphic::Renderer2D_AddLine(glm::vec3(contact_data.pos, 1), contact_data.normal);
        }
        if (contact_data_overlap.hit) {
            sk_graphic::Renderer2D_AddLBox(
                glm::vec3(contact_data_overlap.pos - dbox.size / 2.0f, 0),
                dbox.size,
                glm::vec4(0.1, 1, 0.3, 1));
            sk_graphic::Renderer2D_AddLine(glm::vec3(contact_data_overlap.pos, 1), contact_data_overlap.normal);
        }
    }
}