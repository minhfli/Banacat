#include "Level.h"

#include<iostream>

#include <sk_engine/Common/Common.h>

#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Graphics/Camera.h>
#include <sk_engine/Physics/AABB.h>

#include "Entities/AllEntity.h"

#include "Area.h"

void Level::Init() {
    std::string level_path = get_path();
    std::cout << level_path << '\n';
    DATA = ReadJsonFile(level_path);

    std::cout << "level json data loaded\n";

    level_iid = DATA["iid"];

    pos_topleft.x = (int)DATA["worldX"] / 8;
    pos_topleft.y = -(int)DATA["worldY"] / 8;

    m_tilemap.tile_set.Load("Assets/TileSet.png");

    std::cout << "Level Initialize done: " << level_name << '\n';
}
void Level::Load() {
    std::cout << "Loading level: " << level_name << '\n';
    for (nlohmann::json layer : DATA["layerInstances"]) {
        if (layer["__identifier"] == "entities") {
            for (nlohmann::json jentity : layer["entityInstances"]) {
                LoadEntity(jentity);
            }
            continue;
        }
        if (layer["__identifier"] == "basicgound_layer") {
            m_tilemap.width = layer["__cWid"];
            m_tilemap.height = layer["__cHei"];

            m_tilemap.grid_size = layer["__gridSize"];
            m_tilemap.tile_count = layer["autoLayerTiles"].size();

            m_tilemap.tiles.reserve(m_tilemap.tile_count);
            for (int i = 0;i <= m_tilemap.tile_count - 1; i++) {
                m_tilemap.tiles.emplace_back();
                Tile_data& cur_tile = m_tilemap.tiles[i];

                //* set tile's position
                cur_tile.pos.x = (float)layer["autoLayerTiles"][i]["px"][0] / m_tilemap.grid_size + 0.5f + pos_topleft.x; // 0.5 : offset
                cur_tile.pos.y = -(float)layer["autoLayerTiles"][i]["px"][1] / m_tilemap.grid_size - 0.5f + pos_topleft.y;

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
            continue;
        }
        if (layer["__identifier"] == "static_colliders") {
            //if (physic_world == nullptr) std::cout << "f\n";
            nlohmann::json collider_csv = layer["intGridCsv"];
            //static_collider_list.clear();
            for (int i = 0; i <= m_tilemap.height - 1; i++) {
                for (int j = 0; j <= m_tilemap.width - 1; j++) {
                    int csv_index = i * m_tilemap.width + j;
                    if (collider_csv[csv_index] == 1) {
                        if (i == 0 || i == m_tilemap.height - 1 || j == 0 || j == m_tilemap.width - 1 || // check if is edge
                            collider_csv[csv_index - 1] != 1 ||                     // check if there is air next to it
                            collider_csv[csv_index + 1] != 1 ||
                            collider_csv[csv_index - m_tilemap.width] != 1 ||
                            collider_csv[csv_index + m_tilemap.width] != 1
                        )
                            LoadStaticBody(1, pos_topleft + glm::vec2(j, -i));
                    }
                    else if (collider_csv[csv_index] != 0)
                        LoadStaticBody((int)collider_csv[csv_index], pos_topleft + glm::vec2(j, -i));
                }
            }
            continue;
        }
    }
    Loaded = true;
    std::cout << "Level loaded: " << level_name << "\n";
}
void Level::UnLoad() {
    std::cout << "Unloading level: " << level_name << '\n';

    m_tilemap.tiles.clear();
    for (int index : static_collider_list) physic_world->Remove_Body(index);
    static_collider_list.clear();

    UnLoadEntity();
    Loaded = false;
}

void Level::LoadStaticBody(int type, glm::vec2 body_topleft_pos) {
    int tag = 0;

    glm::vec2 body_lowleft_pos = body_topleft_pos + glm::vec2(0, -1);
    float spike_width = 0.3f;
    switch (type) {
        case 1: //? normal tile collider
            AddTag(tag, etag::PHY_SOLID);
            AddTag(tag, etag::GROUND);
            static_collider_list.emplace_back(
            physic_world->Create_Body(sk_physic2d::Body_Def(
                sk_physic2d::irect::irect_fray(body_topleft_pos, glm::vec2(1, -1)),
                tag
            )));
            break;
        case 2: //? oneway tile collider
            AddTag(tag, etag::PHY_SOLID);
            AddTag(tag, etag::PHY_ONE_WAY);
            static_collider_list.emplace_back(
            physic_world->Create_Body(sk_physic2d::Body_Def(
                sk_physic2d::irect::irect_fray(body_topleft_pos, glm::vec2(1, -1)),
                tag
            )));
            break;
        case 3: //? spike, point up
            AddTag(tag, etag::PHY_TRIGGER);
            AddTag(tag, etag::PHY_DIR_U);
            AddTag(tag, etag::DAMAGE);
            static_collider_list.emplace_back(
            physic_world->Create_Body(sk_physic2d::Body_Def(
                sk_physic2d::irect::irect_fray(body_lowleft_pos, glm::vec2(1, spike_width)),
                tag
            )));
            break;
        case 4: //? spike, point left
            AddTag(tag, etag::PHY_TRIGGER);
            AddTag(tag, etag::PHY_DIR_L);
            AddTag(tag, etag::DAMAGE);
            static_collider_list.emplace_back(
            physic_world->Create_Body(sk_physic2d::Body_Def(
                sk_physic2d::irect::irect_fray(body_lowleft_pos + glm::vec2(1, 0), glm::vec2(-spike_width, 1)),
                tag
            )));
            break;
        case 5: //? spike, point right
            AddTag(tag, etag::PHY_TRIGGER);
            AddTag(tag, etag::PHY_DIR_R);
            AddTag(tag, etag::DAMAGE);
            static_collider_list.emplace_back(
            physic_world->Create_Body(sk_physic2d::Body_Def(
                sk_physic2d::irect::irect_fray(body_lowleft_pos, glm::vec2(spike_width, 1)),
                tag
            )));
            break;
        case 6: //? spike, point down
            AddTag(tag, etag::PHY_TRIGGER);
            AddTag(tag, etag::PHY_DIR_D);
            AddTag(tag, etag::DAMAGE);
            static_collider_list.emplace_back(
            physic_world->Create_Body(sk_physic2d::Body_Def(
                sk_physic2d::irect::irect_fray(body_lowleft_pos + glm::vec2(0, 1), glm::vec2(1, -spike_width)),
                tag
            )));
            break;
        default:
            break;
    }
}

void Level::LoadEntity(nlohmann::json jentity) {
    if (jentity["__identifier"] == "camerabound") {
        special_level_data.camera_bound =
            glm::vec4(pos_topleft, pos_topleft) +
            glm::vec4(
                (int)jentity["px"][0],
                -(int)jentity["px"][1] - (int)jentity["height"],
                (int)jentity["px"][0] + (int)jentity["width"],
                -(int)jentity["px"][1]
            ) / (float)grid_size;
        return;
    }
    if (jentity["__identifier"] == "player_spawn") {
        PlayerSpawn* e = new PlayerSpawn();
        m_entity.emplace_back(e);
        e->OnJsonCreate(m_area, this, jentity);
        return;
    }
    if (jentity["__identifier"] == "dash_crystal") {
        DashCrystal* e = new DashCrystal();
        m_entity.emplace_back(e);
        e->OnJsonCreate(m_area, this, jentity);
        return;
    }
    if (jentity["__identifier"] == "spring") {
        Spring* e = new Spring();
        m_entity.emplace_back(e);
        e->OnJsonCreate(m_area, this, jentity);
        return;
    }
}
void Level::UnLoadEntity() {
    for (Entity* e : m_entity) {
        e->OnDestroy();
        delete e;
    }
    m_entity.clear();
}
void Level::OnActive() {
    // set camera bound
    auto cam = sk_graphic::Renderer2D_GetCam();
    cam->focus.cambound = special_level_data.camera_bound;
    // set player default spawn point
    m_area->GetPlayer()->SetSpawnPoint(special_level_data.default_spawn_point);

    for (auto e : m_entity) e->OnActive();
}
void Level::OnDeActive() {
    for (auto e : m_entity) e->OnDeActive();
}

void Level::Update() {
    for (auto e : m_entity) e->Update();

}
void Level::Draw() {
    for (int i = 0;i <= m_tilemap.tile_count - 1; i++) {
        Tile_data& cur_tile = m_tilemap.tiles[i];

        sk_graphic::Renderer2D_AddQuad(
            glm::vec2(cur_tile.pos.x, cur_tile.pos.y),
            glm::vec2(1),
            0,
            cur_tile.uv,
            m_tilemap.tile_set,
            glm::vec4(1)
        );
    }

    for (Entity* e : m_entity) {
        e->Draw();
        if (e->CheckTag_(etag::SPAWN_POINT))
            sk_graphic::Renderer2D_AddDotX(glm::vec3((((PlayerSpawn*)e)->spawn_point), 1), { 1,1,0,1 });
    }
}
void Level::LateUpdate() {}