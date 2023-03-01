#pragma once

#include <GLM/glm.hpp>

#include "Vertex2D.h"
#include "Camera.h"

//? batch renderer 
namespace sk_graphic {

    void Renderer2D_Init();
    void Renderer2D_ShutDown();

    //* Sprite batch use for draw quads
    void Renderer2D_Begin();
    void Renderer2D_End();
    void Renderer2D_AddQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color = glm::vec4(1.0f));
    void Renderer2D_AddLine(glm::vec3 pos, glm::vec2 dir);
    void Renderer2D_FlushQuads();
    void Renderer2D_FlushLines();

    //! Temporary
    void Renderer2D_GetCam(Camera* cam);
}