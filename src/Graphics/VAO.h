#pragma once

#include <GLAD/glad.h>

#include "VBO.h"

/// @brief Vertex Array
struct VAO { //? vertex array object 
    public:

    GLuint ID = 0;
    //VAO();
    void Create();
    void link_attrib(VBO& vbo, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void UnBind();
    void Delete();
};

