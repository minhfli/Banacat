#include "Sprite2D.h"
#include "2D_Renderer.h"

Sprite2D::Sprite2D() {}

void Sprite2D::load_texture(const Texture2D& texture, const glm::vec4& uv) {
    this->texture_id = texture.ID;
    this->tex_uv = uv;
    this->size = texture.size * (glm::vec2(uv.z, uv.w) - glm::vec2(uv.x, uv.y));
}


//void draw_sprite(const Sprite2D& sprite, const glm::vec3& pos = glm::vec3(0)) {}

inline void Sprite2D::Draw(const glm::vec3& pos, const glm::vec4& color) {
    Renderer2D_AddQuad(pos, this->size, color);
}
