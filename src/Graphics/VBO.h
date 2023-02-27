#pragma once

#include <GLAD/glad.h>

GLuint CreateVBO();
void vbo_Add_Data(GLuint id, const void* vertices, GLuint size, GLenum usage);
void vbo_Update_Data(GLuint id, const void* vertices, GLuint size);
void vbo_Bind(GLuint id);
void vbo_Delete(GLuint& id);

class VertexBuffer {
    public:
    GLuint id;

    VertexBuffer();
    ~VertexBuffer();

    void Set_Data(const void* vertices, GLuint size, GLenum usage);
    void Upd_Data(const void* vertices, GLuint size);
    void Bind();
    void Delete();

};
