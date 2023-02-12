#pragma once

#include <GLAD/glad.h>

GLuint CreateEBO();
void ebo_Add_Data(GLuint id, GLuint* indices, GLuint size, GLenum usage);
void ebo_Update_Data(GLuint id, GLuint* indices, GLuint size);
void ebo_Bind(GLuint id);
void ebo_Delete(GLuint id);
