#include "Sprite2D.h"


void Sprite2D_core::load_texture(GLuint id, glm::vec4 uv) {
    texture_id = id;
    tex_uv = uv;
}
