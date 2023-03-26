#include "Test.h"
#include <Graphics/2D_Renderer.h>

namespace sk_physic2d {

    rect sbox(glm::vec2(-1), glm::vec2(6, 2));
    rect dbox(glm::vec2(-3), glm::vec2(1, 1));
    contact contact_data;
    contact contact_data_overlap;
    glm::vec3 mouse_pos;
    void Setup() {
    }
    void Update(uint32_t delta_time, Camera& cam) {
        mouse_pos = cam.Screen_To_World(sk_input::MousePos(), glm::vec2(800, 600));
        glm::vec2 dbox_velocity;
        dbox_velocity.x = mouse_pos.x - dbox.pos.x;
        dbox_velocity.y = mouse_pos.y - dbox.pos.y;

        if (sk_input::Key(sk_key::SPACE)) {
            dbox.pos = mouse_pos;
        }
        swept_rect_vs_rect(dbox, sbox, dbox_velocity, &contact_data);
        colli_rect_vs_rect(dbox, sbox, &contact_data_overlap);

    }
    void Draw() {
        glm::vec2 dbox_velocity;

        dbox_velocity.x = mouse_pos.x - dbox.pos.x;
        dbox_velocity.y = mouse_pos.y - dbox.pos.y;

        sk_graphic::Renderer2D_AddLine(glm::vec3(dbox.pos, 1), dbox_velocity, glm::vec4(1, 1, 0, 1));

        sk_graphic::Renderer2D_AddBBox(dbox.bound(), 0, glm::vec4(0, 1, 0, 1));
        sk_graphic::Renderer2D_AddBBox(sbox.bound(), 0, glm::vec4(0, 1, 0, 1));
        if (contact_data.hit) {
            sk_graphic::Renderer2D_AddDotX(glm::vec3(contact_data.pos, 2));
            // draw contact rect
            sk_graphic::Renderer2D_AddBBox(
                glm::vec4(contact_data.pos - dbox.hsize, contact_data.pos + dbox.hsize),
                0,
                glm::vec4(0, 1, 0, 1));
            // draw normal
            sk_graphic::Renderer2D_AddLine(glm::vec3(contact_data.pos, 1), contact_data.normal);
        }
        if (contact_data_overlap.hit) {
            sk_graphic::Renderer2D_AddBBox(
                glm::vec4(contact_data_overlap.pos - dbox.hsize, contact_data_overlap.pos + dbox.hsize),
                0,
                glm::vec4(0.1, 1, 0.3, 1));
            sk_graphic::Renderer2D_AddLine(glm::vec3(contact_data_overlap.pos, 1), contact_data_overlap.normal);
        }
    }
}