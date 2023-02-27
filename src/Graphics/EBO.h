#pragma once

#include <GLAD/glad.h>

GLuint CreateEBO();
void ebo_Add_Data(GLuint id, GLuint* indices, GLuint size, GLenum usage);
void ebo_Update_Data(GLuint id, GLuint* indices, GLuint size);
void ebo_Bind(GLuint id);
void ebo_Delete(GLuint& id);

class IndexBuffer {
    public:
    GLuint id;

    IndexBuffer();
    ~IndexBuffer();

    void Set_Data(const void* indices, GLuint size, GLenum usage);
    void Upd_Data(const void* indices, GLuint size);
    void Bind();
    void Delete();

};
typedef IndexBuffer ElementBuffer;
