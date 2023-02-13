#pragma once

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:

    GLuint ID = 0;
    Shader() {};

    /// @brief create and compile
    void Compile(const char* vertFile, const char* fragFile);

    void    SetFloat(const char* name, float value, bool useShader = false);
    void    SetInteger(const char* name, int value, bool useShader = false);
    void    SetVector2f(const char* name, float x, float y, bool useShader = false);
    void    SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
    void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
    void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
    void    SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
    void    SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
    void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

    void Use();
    void Delete();
};

//GLuint createShader();
//void CompileShader(GLenum type, std::string path, GLenum id);