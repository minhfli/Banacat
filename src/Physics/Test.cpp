#include "Test.h"
#include <Graphics/2D_Renderer.h>

namespace sk_physic2d {

    rect box(glm::vec2(-2), glm::vec2(6, 2));
    ray  line(glm::vec2(-3, -4), glm::vec2(6, 3));
    contact contact_data;
    void Setup() {
        ray_vs_rect(line, box, &contact_data);

    }
    void Update(uint32_t delta_time) {
    }
    void Draw() {
        sk_graphic::Renderer2D_AddLine(glm::vec3(line.pos, 1), line.dir, glm::vec4(1, 1, 0, 1));
        sk_graphic::Renderer2D_AddLBox(glm::vec3(box.pos, 0), box.size, glm::vec4(0, 1, 0, 1));
        if (contact_data.hit) {
            sk_graphic::Renderer2D_AddDotX(glm::vec3(contact_data.pos, 2));
            sk_graphic::Renderer2D_AddLine(glm::vec3(contact_data.pos, 1), contact_data.normal);
        }
    }
}