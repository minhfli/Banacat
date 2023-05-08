#pragma once

#include "GLM/glm.hpp"

#include <sk_engine/Graphics/Texture2D.h>
#include <vector>
#include <nlohmann/json.hpp>

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
    std::vector<Tile_data> tiles;
    sk_graphic::Texture2D tile_set;

    int width, height;
    /// @brief grid size in pixel
    int grid_size = 8;

    //float depth = 0;

    void LoadLayer(const nlohmann::json jlayer, const glm::vec2 level_topleft_pos);
    void LoadTiles(const nlohmann::json jtiles, const glm::vec2 level_topleft_pos);
    void Draw(float depth);
};
