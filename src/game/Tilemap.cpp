#include "Tilemap.h"
#include "nlohmann/json.hpp"

#include <iostream>
#include <sk_engine/Graphics/Graphics.h>

void Tilemap::LoadLayer(const nlohmann::json jlayer, const glm::vec2 level_topleft_pos) {
    width = jlayer["__cWid"];
    height = jlayer["__cHei"];

    grid_size = jlayer["__gridSize"];

    if (!jlayer["autoLayerTiles"].is_null() && jlayer["autoLayerTiles"].size() != 0)
        LoadTiles(jlayer["autoLayerTiles"], level_topleft_pos);
    if (!jlayer["gridTiles"].is_null() && jlayer["gridTiles"].size() != 0)
        LoadTiles(jlayer["gridTiles"], level_topleft_pos);

}
void Tilemap::LoadTiles(const nlohmann::json jtiles, const glm::vec2 level_topleft_pos) {
    tile_count = jtiles.size();
    tiles.assign(tile_count, Tile_data());
    for (int i = 0;i <= tile_count - 1; i++) {
        Tile_data& cur_tile = tiles[i];

        //* set tile's position
        cur_tile.pos.x = (float)jtiles[i]["px"][0] / grid_size + 0.5f + level_topleft_pos.x; // 0.5 : offset
        cur_tile.pos.y = -(float)jtiles[i]["px"][1] / grid_size - 0.5f + level_topleft_pos.y;

        //* set tile's texture coordinate
        cur_tile.uv.x = jtiles[i]["src"][0];
        cur_tile.uv.y = jtiles[i]["src"][1];

        cur_tile.uv.z = cur_tile.uv.x + grid_size;
        cur_tile.uv.w = cur_tile.uv.y + grid_size;

        //* set tile's flip 
        int flip = jtiles[i]["f"];
        if ((flip & 1)) std::swap(cur_tile.uv.x, cur_tile.uv.z);    // flip x
        if ((flip & 2)) std::swap(cur_tile.uv.y, cur_tile.uv.w);    // flip y
    }
}
void Tilemap::Draw(float depth) {
    for (Tile_data& tile : tiles) {
        sk_graphic::Renderer2D_AddQuad(
            glm::vec2(tile.pos.x, tile.pos.y),
            glm::vec2(1),
            depth,
            tile.uv,
            tile_set,
            glm::vec4(1)
        );
    }
}