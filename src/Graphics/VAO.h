#pragma once

#include <GLAD/glad.h>

GLuint CreateVAO();
void vao_link_attrib(GLuint vbo_id, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset);
void vao_Bind(GLuint id);
void vao_Delete(GLuint& id);

class VertexArray {
    GLuint id;

    VertexArray();
    ~VertexArray();

    void Gen();
    void Bind() const;
    void Attrib(GLuint vbo_id, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset);
    void Delete();
};