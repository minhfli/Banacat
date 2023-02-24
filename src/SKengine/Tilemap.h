#pragma once

#include <Graphics/Sprite2D.h>
#include <Graphics/Texture2D.h>
#include <Graphics/2D_Renderer.h>

class Tilemap {
    public:
    Tilemap();
    ~Tilemap();

    Texture2D tileset;
    Sprite2D sprites[100][100];

    //? size of the filemap
    uint32_t x, y;

    void Draw();

};