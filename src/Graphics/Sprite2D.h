#pragma once

#include <GLM/glm.hpp>
#include <GLAD/glad.h>

#include "Texture2D.h"

class Sprite2D_core {
    public:
    glm::vec3 pos = glm::vec3(0);

    GLuint texture_id;
    glm::vec4 tex_uv;

    void load_texture(GLuint id, glm::vec4 uv);

};