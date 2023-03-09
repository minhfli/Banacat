#include "VBO.h"
#include <iostream>

VertexBuffer::VertexBuffer() {}
VertexBuffer::~VertexBuffer() {}

void VertexBuffer::Gen() {
    glGenBuffers(1, &id);
}

void VertexBuffer::Set_Data(const void* vertices, GLuint size, GLenum usage) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

void VertexBuffer::Upd_Data(const void* vertices, GLuint size) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

void VertexBuffer::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Delete() {
    glDeleteBuffers(1, &id);
}
