#pragma once

#include <GLAD/glad.h>
#include "VBO.h"

class VertexArray {
    public:
    GLuint id = 0;

    VertexArray();
    ~VertexArray();

    void Gen();
    void Bind();
    void Attrib(VertexBuffer& vbo, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset);
    void Delete();
};