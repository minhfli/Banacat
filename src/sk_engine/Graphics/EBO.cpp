#include "EBO.h"

#include <iostream>

IndexBuffer::IndexBuffer() {}
IndexBuffer::~IndexBuffer() {}

void IndexBuffer::Gen() {
    glGenBuffers(1, &id);
}

void IndexBuffer::Set_Data(const void* indices, GLuint size, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}

void IndexBuffer::Upd_Data(const void* indices, GLuint size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices);
}

void IndexBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Delete() {
    glDeleteBuffers(1, &id);
}
