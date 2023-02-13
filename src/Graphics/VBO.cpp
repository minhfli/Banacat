#include "VBO.h"
#include <iostream>

GLuint CreateVBO() {
    GLuint id;
    glGenBuffers(1, &id);
    return id;
}

/// @brief create memory for the vertex buffer
void vbo_Add_Data(GLuint id, GLfloat* vertices, GLuint size, GLenum usage) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}
/// @brief only for DYNAMIC buffers
void vbo_Update_Data(GLuint id, GLfloat* vertices, GLuint size) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}
void vbo_Bind(GLuint id) {
    glBindBuffer(GL_ARRAY_BUFFER, id);

}
void vbo_Delete(GLuint& id) {
    glDeleteBuffers(1, &id);
}

