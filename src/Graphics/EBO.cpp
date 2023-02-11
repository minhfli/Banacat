#include "EBO.h"

#include <iostream>

void EBO::Create() {
    if (ID != 0) {
        std::cout << "ERROR::EBO already created   ID::" << ID;
        return;
    }
    glGenBuffers(1, &ID);
}

void EBO::AddData(GLuint* indices, GLuint size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
}

void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::UnBind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}

