#pragma once

#define GLM_FORCE_SWIZZLE //enable swizzle for glm
#include <GLM/glm.hpp>

#include "Vertex2D.h"
#include "Texture2D.h"
#include "Camera.h"

//? batch renderer 
namespace sk_graphic {

    void Renderer2D_Init();
    void Renderer2D_ShutDown();

    //* Sprite batch use for drawing shapes
    void Renderer2D_Begin();
    void Renderer2D_End();

    //* main function to add shapes to batch

    /// @brief Add quad with texture
    /// @param pos 
    /// @param size 
    /// @param id texture id
    /// @param uv texture coordinate, real pixel texture coordinate
    /// @param color 
    /// @param texture pointer to texture, if null pointer, set to default error texture
    void Renderer2D_AddQuad(
        const glm::vec3& pos,
        const glm::vec2& size,
        const glm::vec4& uv,
        const glm::vec4& color = glm::vec4(1),
        Texture2D* texture = nullptr
    );
    void Renderer2D_AddLine(
        const glm::vec3& pos,
        const glm::vec2& size,
        const glm::vec4& color = glm::vec4(1));

    //* some other helpful function
    void Renderer2D_AddLBox(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color = glm::vec4(1));
    void Renderer2D_AddDotX(const glm::vec3& pos, const glm::vec4& color = glm::vec4(1.0f));
    void Renderer2D_AddDotO(const glm::vec3& pos, const glm::vec4& color = glm::vec4(1.0f));

    //* funtion to draw to screen
    void Renderer2D_FlushQuads();
    void Renderer2D_FlushLines();

    //! Temporary
    Camera* Renderer2D_GetCam();
}