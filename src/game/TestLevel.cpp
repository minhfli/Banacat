#include "TestLevel.h"
#include "Tilemap.h"

#include <sk_engine/Common/Common.h>
#include <iostream>

namespace sk_game {
    namespace test_level {

        namespace { //private data
            Tilemap tilemap_;

        }
        void LoadLevel() {
            nlohmann::json file = ReadJsonFile(level_path);
            nlohmann::json data = file["layerInstances"][1];

            tilemap_.tile_set.Load(tileset_path);

            tilemap_.width = data["__cWid"];
            tilemap_.height = data["__cHei"];

            tilemap_.grid_size = data["__gridSize"];

            int tile_count = data["autoLayerTiles"].size();

            tilemap_.tiles = new Tile_data[tile_count];

            for (int i = 0;i <= tile_count - 1; i++) {
                Tile_data& cur_tile = tilemap_.tiles[i];

                //* set tile's position
                cur_tile.pos.x = (float)data["autoLayerTiles"][i]["px"][0] / tilemap_.grid_size;
                cur_tile.pos.y = (float)data["autoLayerTiles"][i]["px"][1] / tilemap_.grid_size;

                //* set tile's texture coordinate
                cur_tile.tex.x = (float)data["autoLayerTiles"][i]["src"][0] / tilemap_.tile_set.size.x;
                cur_tile.tex.y = (float)data["autoLayerTiles"][i]["src"][1] / tilemap_.tile_set.size.y;

                cur_tile.tex.z = cur_tile.tex.x + (float)tilemap_.grid_size / tilemap_.tile_set.size.x;
                cur_tile.tex.w = cur_tile.tex.y + (float)tilemap_.grid_size / tilemap_.tile_set.size.y;

                //* set tile's flip bit
                cur_tile.flip = data["autoLayerTiles"][i]["f"];
            }
        }
        void Draw() {

        }
    }
}