#pragma once

#include <GLAD/glad.h>

#include <GLM/glm.hpp>

#include "Shader.h"

class Camera {
    public:

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    bool lock_target = false;
    glm::vec3* target;

    glm::vec3 up = glm::vec3(0.0f, 0.1, 0.0f);

    int width, height;

    /// @brief only for orthographic camera 
    float size = 1;
    float aspect;

    glm::mat4 projection;

    void ProjectionP(float FOV, int width, int height);
    void ProjectionO(float size, int width, int height);

    void CamMatrix(GLSLprogram& shader);
};