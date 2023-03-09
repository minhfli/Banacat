#pragma once

#include <GLAD/glad.h>

class VertexBuffer {
    public:
    GLuint id = 0;

    VertexBuffer();
    ~VertexBuffer();

    void Gen();
    void Set_Data(const void* vertices, GLuint size, GLenum usage);
    void Upd_Data(const void* vertices, GLuint size);
    void Bind();
    void Delete();

};
