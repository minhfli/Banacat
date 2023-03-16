#pragma once

#include "GLM/glm.hpp"

#include <sk_engine/Graphics/Texture2D.h>

struct Tile_data {
    /// @brief texture coordinate, real pixel coordinate
    glm::ivec4 uv;
    /// @brief tile position is in down right order
    glm::vec2 pos;

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
