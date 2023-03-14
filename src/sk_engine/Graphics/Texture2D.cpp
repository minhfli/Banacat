#include "Texture2d.h"

#include <STB/stb_image.h>
#include <iostream>
#include <Common/Common.h>

namespace sk_graphic {
    Texture2D::Texture2D() {}

    void Texture2D::Load(const std::string path) {

        // check if file name path is loaded ?
        // if yes copy texture data to this 
        // if no  create new texture data
        Texture2D* new_texture;
        if (File_Manager::find_string_p(path, (void**)&new_texture)) {
            *this = *new_texture;
            return;
        }

        //?load image data
        int width, height, Channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &Channels, 0);
        if (!data) FatalError("stb:: cant load image" + path);
        std::cout << width << " " << height << " " << Channels << '\n';

        //? create a texture
        glGenTextures(1, &this->ID);
        //texture_id[path] = ID;
        glBindTexture(GL_TEXTURE_2D, this->ID);

        //? texture settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //? copy image data to texture
        if (Channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            std::cout << "tex loaded\n";
        }
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        this->size.x = (float)width;
        this->size.y = (float)height;

        //? free image data
        stbi_image_free(data);

        new_texture = new Texture2D(*this);
        File_Manager::add_string_p(path, (void**)&new_texture);

        //?return texture id
        //return ID;
    }

    void Texture2D::Bind(GLuint slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, this->ID);
    }
    void Texture2D::Delete() {
        glDeleteTextures(1, &this->ID);
    }
}