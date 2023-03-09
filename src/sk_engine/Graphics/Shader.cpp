

#include "Shader.h"
#include <Common/ReadFile.h>
#include <Common/Error.h>

#include <string>

int  success;
char infoLog[512];

void checkCompileError(GLuint shader, std::string file) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        FatalError("ERROR::SHADER::COMPILATION_FAILED\nFile: " + file + '\n' + (std::string)infoLog + '\n');
    }
}

void GLSLprogram::Gen() {
    id = glCreateProgram();
}
void GLSLprogram::Compile(const std::string file, GLenum type) {
    GLuint shader_id = glCreateShader(type);

    std::string shader_source = ReadTextFile(file);
    const char* shader_src = shader_source.c_str();

    glShaderSource(shader_id, 1, &shader_src, NULL);
    glCompileShader(shader_id);

    checkCompileError(shader_id, file);

    glAttachShader(this->id, shader_id);

    glDeleteShader(shader_id);
}
void GLSLprogram::Link() {
    glLinkProgram(id);
}
void GLSLprogram::Use() {
    if (id == 0) FatalError("GLSLprogram program not created");
    glUseProgram(id);
}

void GLSLprogram::Delete() {
    glDeleteProgram(id);
}

void GLSLprogram::SetFloat(const char* name, float value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->id, name), value);
}
void GLSLprogram::SetInteger(const char* name, int value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->id, name), value);
}
void GLSLprogram::SetVector2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->id, name), x, y);
}
void GLSLprogram::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}
void GLSLprogram::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}
void GLSLprogram::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}
void GLSLprogram::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}
void GLSLprogram::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}
void GLSLprogram::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}
