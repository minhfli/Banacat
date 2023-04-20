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
        // update Focus
        if (focus.update_mode == CamMode::UPDATE_FIXED)
            this->position = glm::vec3(focus.pos + focus.offset, 0);
        if (focus.update_mode == CamMode::UPDATE_SMOOTH) {

            glm::vec2 target = focus.pos + focus.offset;

            float lerp_x = focus.damping.x * sk_time::delta_time;
            float lerp_y = focus.damping.y * sk_time::delta_time;

            position.x += (target.x - position.x) * lerp_x;
            position.y += (target.y - position.y) * lerp_y;

            position.x = std::clamp(position.x, target.x - focus.softzone.x, target.x + focus.softzone.x);
            position.y = std::clamp(position.y, target.y - focus.softzone.y, target.y + focus.softzone.y);
        }
        position.x = std::min(position.x, focus.cambound.z - focus.cam_hsize.x);
        position.x = std::max(position.x, focus.cambound.x + focus.cam_hsize.x);
        position.y = std::min(position.y, focus.cambound.w - focus.cam_hsize.y);
        position.y = std::max(position.y, focus.cambound.y + focus.cam_hsize.y);
    }
    void Camera::Draw() {
        Renderer2D_AddBBox(focus.cambound, 2);
    }
}