#pragma once

#include <GLAD/glad.h>

/// @brief Vertex Buffer
class VBO { //? vertex buffer object 
    public:

    GLuint ID = 0;

    void Create();
    void Add_Data(GLfloat* vertices, GLuint size, GLenum usage);
    void Update_Data(GLfloat* vertices, GLuint size);
    void Bind();
    void UnBind();
    void Delete();
};

