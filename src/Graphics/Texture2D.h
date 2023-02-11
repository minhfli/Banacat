#pragma once 
#include <string>
#include <unordered_map>

#include <GLAD/glad.h>

class Texture2D {

    private:
    //static std::unordered_map<std::string, GLuint> texture_id;

    public:

    GLuint ID;

    Texture2D();
    void Load(std::string path);
    void Bind();
    void UnBind();
    void Delete();
};