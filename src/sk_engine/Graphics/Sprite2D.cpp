#include "Sprite2D.h"
#include "2D_Renderer.h"

namespace sk_graphic {

    Sprite2D::Sprite2D() {}

    void Sprite2D::LoadTexture(Texture2D texture, glm::vec2 s_size, glm::ivec4 uv) {
        this->texture_id = texture.ID;

        if (uv.x == -1)
            this->tex_uv = glm::ivec4(0, 0, texture.size);
        else
            this->tex_uv = uv;
        if (s_size.x == -1)
            this->size = glm::vec2(abs(uv.z - uv.x), abs(uv.w - uv.y));
        else
            this->size = s_size;
    }
    void Sprite2D::Draw(glm::vec2 pos, float depth, glm::vec2 pivot, bool flipx, bool flipy, glm::vec4 color) {
        glm::vec4 uv = tex_uv;
        if (flipx) std::swap(uv.x, uv.z);
        if (flipy) std::swap(uv.y, uv.w);
        Renderer2D_AddQuad(
            glm::vec4(pos - pivot * size, pos - pivot * size + size),
            depth,
            uv,
            Texture2D(texture_id),
            color);
    }
}
