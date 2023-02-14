#include "Texture2d.h"

#include <STB/stb_image.h>
#include <iostream>
#include <Common/Error.h>

GLuint tex_Load(const std::string path) {

    //if (texture_id.find(path) != texture_id.end()) return;

    //?load image data
    int width, height, Channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &Channels, 0);
    if (!data) FatalError("stb:: cant load image" + path);
    std::cout << width << " " << height << " " << Channels << '\n';

    GLuint ID;
    //? create a texture
    glGenTextures(1, &ID);
    //texture_id[path] = ID;
    glBindTexture(GL_TEXTURE_2D, ID);

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
    //? free image data
    stbi_image_free(data);

    //?return texture id
    return ID;
}

void tex_Bind(GLuint id, GLuint slot) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE0, id);
}
void tex_Delete(GLuint id) {
    glDeleteTextures(1, &id);
}