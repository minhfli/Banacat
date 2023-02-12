#pragma once

#include <GLM/glm.hpp>
#include <GLAD/glad.h>

struct vertex {
    glm::vec3 pos;
    glm::vec4 color = glm::vec4(1);
    glm::vec2 uv;     //? texture coord
    GLuint texture;   //? texture index
};

