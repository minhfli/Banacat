#pragma once

#include <GLM/glm.hpp>
#include <GLAD/glad.h>

#include "Texture2D.h"

class Sprite2D {
    public:
    GLuint texture_id = 0;
    glm::vec4 tex_uv = glm::vec4(0);
    glm::vec2 size = glm::vec2(0);

    Sprite2D();

    void load_texture(const Texture2D& texture, const glm::vec4& uv);
    inline void Draw(const glm::vec3& pos, const glm::vec4& color = glm::vec4(1));
};

//void draw_sprite(const Sprite2D& sprite, const glm::vec3& pos = glm::vec3(0));