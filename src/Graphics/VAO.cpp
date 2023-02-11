#include "VAO.h"
#include <iostream>

/*VAO::VAO() {
    glGenVertexArrays(1, &ID);
}*/

void VAO::Create() {
    if (ID != 0) {
        std::cout << "ERROR::VAO already created   ID::" << ID;
        return;
    }
    glGenVertexArrays(1, &ID);
}

void VAO::link_attrib(VBO& vbo, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset) {
    vbo.Bind();
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, num, type, GL_FALSE, stride, offset);
    vbo.UnBind();
}

void VAO::Bind() {
    glBindVertexArray(ID);
}

void VAO::UnBind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}

