#include "VAO.h"

VertexArray::VertexArray() {}
VertexArray::~VertexArray() {}

void VertexArray::Gen() {
    glGenVertexArrays(1, &id);
}

void VertexArray::Bind() {
    glBindVertexArray(id);
}

void VertexArray::Attrib(VertexBuffer& vbo, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset) {
    /*
        why we have to bind vbo before call attrib pointer ?
        vao is an object describe how vertex data is sent to shader
        different attrib can be bound to different buffer
        bind vbo to make the vao know vbo of the attribute
    */
    vbo.Bind();
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, num, type, GL_FALSE, stride, offset);
}

void VertexArray::Delete() {
    glDeleteVertexArrays(1, &id);
}
