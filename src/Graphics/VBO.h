#pragma once

#include <GLAD/glad.h>

GLuint CreateVBO();
void vbo_Add_Data(GLuint id, GLfloat* vertices, GLuint size, GLenum usage);
void vbo_Update_Data(GLuint id, GLfloat* vertices, GLuint size);
void vbo_Bind(GLuint id);
void vbo_Delete(GLuint id);


