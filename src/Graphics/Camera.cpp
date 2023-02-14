#include "Camera.h"

void Camera::ProjectionP(float FOV, int width, int height) {
    projection = glm::perspective(glm::radians(FOV), (float)width / height, 0.1f, 100.0f);
}

void Camera::ProjectionO(float size, int width, int height) {
    float aspect = (float)width / height;
    projection = glm::ortho(-aspect * size, aspect * size, -size, size, -10.0f, 10.0f);
}

void Camera::CamMatrix(Shader& shader) {
    glm::mat4 view;
    if (lock_target)
        view = glm::lookAt(position, *target, up);
    else view = glm::lookAt(position, position + direction, up);


    shader.SetMatrix4("CamMatrix", projection * view, 1);
}
