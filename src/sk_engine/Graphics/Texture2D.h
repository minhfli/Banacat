#pragma once 
#include <string>
#include <unordered_map>

#include <GLAD/glad.h>
#include <GLM/glm.hpp>

namespace sk_graphic {
    const GLuint PIXEL_PER_UNIT = 16;
    class Texture2D {
        public:
        GLuint ID = 0;

        //?size of the texture
        glm::vec2 size = glm::vec2(0);

        Texture2D();

        void load(const std::string path);
        void Bind(GLuint slot);
        //void UnBind(GLuint slot);
        void Delete();
    };
}