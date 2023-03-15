#pragma once

#include "GLM/glm.hpp"

#include <sk_engine/Graphics/Texture2D.h>

struct Tile_data {
    /// @brief texture coordinate, not texture
    glm::vec4 tex;
    /// @brief tile position is in down right order
    glm::vec2 pos;

    int flip = 0;
    //  "Flip bits", a 2-bits integer to represent the mirror transformations of the tile.
    //  only use if rotate = 0
    //  - Bit 0 = X flip
    //  - Bit 1 = Y flip
    //  Examples: f=0 (no flip), f=1 (X flip only), f=2 (Y flip only), f=3 (both flips)
};
struct Tilemap {
    // tile size is 1x1 (maybe different in  the future)

    // tile list, this is not a 2D array of all tiles of the tilemap, just a list of non null tiles
    int tile_count;
    Tile_data* tiles = nullptr;
    sk_graphic::Texture2D tile_set;

    glm::vec3 pos = glm::vec3(0);

    int width, height;
    /// @brief grid size in pixel
    int grid_size;
};
