#pragma once

#include <GLAD/glad.h>

class IndexBuffer {
    public:
    GLuint id = 0;

    IndexBuffer();
    ~IndexBuffer();

    void Gen();
    void Set_Data(const void* indices, GLuint size, GLenum usage);
    void Upd_Data(const void* indices, GLuint size);
    void Bind();
    void Delete();

};
typedef IndexBuffer ElementBuffer;
