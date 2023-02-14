#pragma once

#include <GLM/glm.hpp>

#include "Vertex2D.h"

//? batch renderer 

void Renderer2D_Init();
void Renderer2D_ShutDown();

//* Sprite batch use for draw quads
void Renderer2D_BeginBatch();
void Renderer2D_EndBatch();
void Renderer2D_AddQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(1.0f));
void Renderer2D_FlushBatch();        //? draw all 

//! Temporary
GLuint GetShaderID();

