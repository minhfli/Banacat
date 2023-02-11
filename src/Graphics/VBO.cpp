#include "VBO.h"
#include <iostream>


void VBO::Create() {
    if (ID != 0) {
        std::cout << "ERROR::VBO already created   ID::" << ID;
        return;
    }
    glGenBuffers(1, &ID);
}

void VBO::Add_Data(GLfloat* vertices, GLuint size, GLenum usage) {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

/// @brief only for DYNAMIC buffers
void VBO::Update_Data(GLfloat* vertices, GLuint size) {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::UnBind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}

