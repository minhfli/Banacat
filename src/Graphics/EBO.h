#pragma once

#include <GLAD/glad.h>

/// @brief Element/Index Buffer
class EBO { //? element buffer object 
    public:

    GLuint ID;
    void Create();
    void AddData(GLuint* indices, GLuint size);
    void Bind();
    void UnBind();
    void Delete();
};
