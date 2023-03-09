#pragma once

#include <string>
#include <GLAD/glad.h>

//Todo    make a file manager

//? return an id( texture, shader, .. ) that loaded from a file path
GLuint filepath_to_id(std::string path);