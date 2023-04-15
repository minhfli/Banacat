#include "Level.h"

#include<iostream>

#include <sk_engine/Common/Common.h>

#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Physics/AABB.h>

void Level::Init() {
    std::string level_path = get_path();
    DATA = ReadJsonFile(level_path);

    level_iid = DATA["iid"];
}
void Level::Load() {
    std::cout << "Loading level: " << level_name;
    m_tilemap.tile_set.Load("Assets/tilemap/Cavernas_by_Adam_Saltsman.png");
    for (nlohmann::json layer : DATA["layerInstances"]) {
        if (layer["__identifier"] == "entities") {
            continue;
        }
        if (layer["__identifier"] == "intgrid_layer") {
            m_tilemap.width = layer["__cWid"];
            m_tilemap.height = layer["__cHei"];

            m_tilemap.grid_size = layer["__gridSize"];
            m_tilemap.tile_count = layer["autoLayerTiles"].size();

            m_tilemap.tiles.reserve(m_tilemap.tile_count);
            for (int i = 0;i <= m_tilemap.tile_count - 1; i++) {
                m_tilemap.tiles.emplace_back();
                Tile_data& cur_tile = m_tilemap.tiles[i];

                //* set tile's position
                cur_tile.pos.x = (float)layer["autoLayerTiles"][i]["px"][0] / m_tilemap.grid_size;
                cur_tile.pos.y = (float)layer["autoLayerTiles"][i]["px"][1] / m_tilemap.grid_size;

                //* set tile's texture coordinate
                cur_tile.uv.x = layer["autoLayerTiles"][i]["src"][0];
                cur_tile.uv.y = layer["autoLayerTiles"][i]["src"][1];

                cur_tile.uv.z = cur_tile.uv.x + m_tilemap.grid_size;
                cur_tile.uv.w = cur_tile.uv.y + m_tilemap.grid_size;

                //* set tile's flip 
                int flip = layer["autoLayerTiles"][i]["f"];
                if ((flip & 1)) std::swap(cur_tile.uv.x, cur_tile.uv.z);    // flip x
                if ((flip & 2)) std::swap(cur_tile.uv.y, cur_tile.uv.w);    // flip y
            }

            nlohmann::json collider_csv = layer["intGridCsv"];
            for (int i = 0; i <= m_tilemap.height - 1; i++) {
                for (int j = 0; j <= m_tilemap.width - 1; j++) {
                    int csv_index = i * m_tilemap.width + j;

                    static_collider_list.emplace_back();
                    if (collider_csv[csv_index] == 1)
                        physic_world->Create_Body(sk_physic2d::Body_Def(
                            sk_physic2d::irect::irect_fsize_fpos(glm::vec2(0.5f), glm::vec2(j, -i)),
                            0
                        ),
                        &static_collider_list.back()
                        );
                }
            }
        }
    }
}
void Level::UnLoad() {
    std::cout << "Unloading level: " << level_name;

    m_tilemap.tiles.clear();
    for (int index : static_collider_list) physic_world->Remove_Body(index);
}

void Level::OnActive() {}
void Level::OnDeActive() {}

void Level::Update() {}
void Level::Draw() {
    for (int i = 0;i <= m_tilemap.tile_count - 1; i++) {
        Tile_data& cur_tile = m_tilemap.tiles[i];

        sk_graphic::Renderer2D_AddQuad(
            glm::vec3(cur_tile.pos.x, -cur_tile.pos.y, 0),
            glm::vec2(1),
            cur_tile.uv,
            glm::vec4(1),
            &m_tilemap.tile_set
        );
    }
}
void Level::LateUpdate() {}