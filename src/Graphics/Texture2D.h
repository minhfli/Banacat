#pragma once 
#include <string>
#include <unordered_map>

#include <GLAD/glad.h>

GLuint tex_Load(const std::string path);
void tex_Bind(GLuint id);
void tex_Delete(GLuint id);