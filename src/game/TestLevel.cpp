#include "TestLevel.h"
#include "Tilemap.h"

#include <sk_engine/Common/Common.h>
#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Physics/AABB_World.h>

#include "Player.h"

namespace sk_game {
    namespace test_level {

        namespace { //private data
            Tilemap tilemap_;
            sk_physic2d::AABB_World physic_world;
            Player player;
        }
        void LoadLevel() {
            nlohmann::json file = ReadJsonFile(level_path);
            nlohmann::json data = file["layerInstances"][1];

            tilemap_.tile_set.Load(tileset_path);

            tilemap_.width = data["__cWid"];
            tilemap_.height = data["__cHei"];

            tilemap_.grid_size = data["__gridSize"];

            tilemap_.tile_count = data["autoLayerTiles"].size();

            tilemap_.tiles = new Tile_data[tilemap_.tile_count];

            for (int i = 0;i <= tilemap_.tile_count - 1; i++) {
                Tile_data& cur_tile = tilemap_.tiles[i];

                //* set tile's position
                cur_tile.pos.x = (float)data["autoLayerTiles"][i]["px"][0] / tilemap_.grid_size;
                cur_tile.pos.y = (float)data["autoLayerTiles"][i]["px"][1] / tilemap_.grid_size;

                //* set tile's texture coordinate
                cur_tile.uv.x = data["autoLayerTiles"][i]["src"][0];
                cur_tile.uv.y = data["autoLayerTiles"][i]["src"][1];

                cur_tile.uv.z = cur_tile.uv.x + tilemap_.grid_size;
                cur_tile.uv.w = cur_tile.uv.y + tilemap_.grid_size;

                //* set tile's flip 
                int flip = data["autoLayerTiles"][i]["f"];
                //  "Flip bits", a 2-bits integer to represent the mirror transformations of the tile.
                //  only use if rotate = 0
                //  - Bit 0 = X flip
                //  - Bit 1 = Y flip
                //  Examples: f=0 (no flip), f=1 (X flip only), f=2 (Y flip only), f=3 (both flips)
                if ((flip & 1)) std::swap(cur_tile.uv.x, cur_tile.uv.z);
                if ((flip & 2)) std::swap(cur_tile.uv.y, cur_tile.uv.w);
            }

            physic_world.Hint_WorldBound(glm::vec2(tilemap_.width / 2, -tilemap_.height / 2) + glm::vec2(0.5f), 48);
            physic_world.Init();
            nlohmann::json collider_csv = data["intGridCsv"];
            for (int i = 0; i <= tilemap_.height - 1; i++) {
                for (int j = 0; j <= tilemap_.width - 1; j++) {
                    int csv_index = i * tilemap_.width + j;

                    sk_physic2d::rect R = sk_physic2d::rect(glm::vec2(j, -i), glm::vec2(1));
                    if (collider_csv[csv_index] == 1)
                        physic_world.Create_Body(sk_physic2d::Body_Def(R));
                }
            }

            sk_physic2d::Body_Def player_bodydef(sk_physic2d::rect(glm::vec4(0, 0, 1, 1)), 1);
            player.m_body = physic_world.Create_Body(player_bodydef);
        }
        void Update() {
            player.Update();
            physic_world.Update();
        }
        void Draw() {
            player.Draw();
            physic_world.Draw();
            for (int i = 0;i <= tilemap_.tile_count - 1; i++) {
                Tile_data& cur_tile = tilemap_.tiles[i];

                sk_graphic::Renderer2D_AddQuad(
                    glm::vec3(cur_tile.pos.x, -cur_tile.pos.y, 0),
                    glm::vec2(1),
                    cur_tile.uv,
                    glm::vec4(1),
                    &tilemap_.tile_set
                );
            }
        }

    }
}