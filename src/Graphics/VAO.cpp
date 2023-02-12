#include "VAO.h"
#include "VBO.h"

GLuint CreateVAO() {
    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}
void vao_link_attrib(GLuint vbo_id, GLuint layout, GLuint num, GLenum type, GLsizeiptr stride, void* offset) {
    vbo_Bind(vbo_id);
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, num, type, GL_FALSE, stride, offset);
}
void vao_Bind(GLuint id) {
    glBindVertexArray(id);
}
void vao_Delete(GLuint id) {
    glDeleteVertexArrays(1, &id);
}
