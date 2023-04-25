#include "Sprite2D.h"
#include "2D_Renderer.h"

namespace sk_graphic {

    Sprite2D::Sprite2D() {}

    void Sprite2D::LoadTexture(Texture2D texture, glm::vec2 s_size, glm::ivec4 uv = glm::ivec4(-1)) {
        if (uv.x == -1)
            this->tex_uv = glm::ivec4(0, 0, texture.size);
        else
            this->tex_uv = uv;
        this->texture_id = texture.ID;
        this->size = s_size;
    }
    void Sprite2D::Draw(glm::vec2 pos, float depth, glm::vec2 pivot, glm::vec4 color) {
        Renderer2D_AddQuad(
            glm::vec4(pos - pivot * size, pos - pivot * size + size),
            depth,
            tex_uv,
            Texture2D(texture_id),
            color);
    }
}
