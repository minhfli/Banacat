#include "Camera.h"

#include "2D_Renderer.h"
#include <Common/sk_time.h>
#include <iostream>
#include <algorithm>

namespace sk_graphic {

    void Camera::ProjectionP(float FOV, int width, int height) {
        projection = glm::perspective(glm::radians(FOV), (float)width / height, 0.1f, 100.0f);
    }

    void Camera::ProjectionO(float size, int width, int height) {
        this->size = size;
        aspect = (float)width / height;
        projection = glm::ortho(-aspect * size, aspect * size, -size, size, -10.0f, 10.0f);
    }

    void Camera::CamMatrix(GLSLprogram& shader) {
        glm::mat4 view = glm::lookAt(position, position + direction, up);
        /*if (lock_target)
            view = glm::lookAt(position, *target, up);
        else view = glm::lookAt(position, position + direction, up);*/
        shader.SetMatrix4("CamMatrix", projection * view, 1);
    }

    glm::vec3 Camera::Screen_To_World(const glm::vec2& pos, const glm::vec2& src_size) const {

        //? screen to nomalize device coordinate (NDC)
        float x = 2.0 * pos.x / src_size.x - 1.0;
        float y = -2.0 * pos.y / src_size.y + 1.0;

        glm::mat4 view = glm::lookAt(position, position + direction, up);

        glm::mat4 VP_Inv = glm::inverse(projection * view);
        glm::vec4 NDC_pos = glm::vec4(x, y, 0, 1);

        glm::vec4 world_pos = VP_Inv * NDC_pos;

        return glm::vec3(world_pos.x, world_pos.y, world_pos.z);
    }

    void Camera::Update() {
        if (focus.is_transitioning()) {
            position = glm::vec3(focus.GetTransition_pos(), 0);
        }
        else {
            glm::vec2 new_pos = focus.GetTarget_pos(glm::vec2(position.x, position.y));
            position.x = new_pos.x;
            position.y = new_pos.y;
        }
    }
    void Camera::Draw() {
        Renderer2D_AddBBox(focus.cambound, 2);
    }
}