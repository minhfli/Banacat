#pragma once

#include <GLM/glm.hpp>
#include <GLAD/glad.h>

#include "Texture2D.h"

namespace sk_graphic {
    /// @brief sprite class is not implemented, use texture instead
    class Sprite2D {
        public:
        GLuint texture_id = 0;
        glm::ivec4 tex_uv = glm::ivec4(0);
        glm::vec2 size = glm::vec2(0);

        Sprite2D();

        void LoadTexture(Texture2D texture, glm::vec2 s_size, glm::ivec4 uv);

        /// @brief 
        /// @param pivot {0,0} lower left, {1,1} upper right
        void Draw(glm::vec2 pos, float depth, glm::vec2 pivot = glm::vec2(0), bool flipx = false, bool flipy = false, glm::vec4 color = glm::vec4(1));

    };

    //void draw_sprite(const Sprite2D& sprite, const glm::vec3& pos = glm::vec3(0));
}