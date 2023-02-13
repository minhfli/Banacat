#include "EBO.h"

#include <iostream>

GLuint CreateEBO() {
    GLuint id;
    glGenBuffers(1, &id);
    return id;
}
void ebo_Add_Data(GLuint id, GLuint* indices, GLuint size, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}
void ebo_Update_Data(GLuint id, GLuint* indices, GLuint size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices);
}
void ebo_Bind(GLuint id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void ebo_Delete(GLuint& id) {
    glDeleteBuffers(1, &id);
}
