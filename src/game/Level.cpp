#include "Level.h"

#include<iostream>

#include <sk_engine/Common/Common.h>

#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Graphics/Camera.h>
#include <sk_engine/Physics/AABB.h>

#include "Entities/AllEntity.h"

#include "Area.h"

void Level::Init() {
    level_iid = DATA["iid"];
    level_name = DATA["identifier"];

    physic_world = m_area->GetPhysicWorld();
    grid_size = m_area->grid_size;

    pos_topleft.x = (int)DATA["worldX"] / 8;
    pos_topleft.y = -(int)DATA["worldY"] / 8;

    size.x = (int)DATA["pxWid"] / 8;
    size.y = (int)DATA["pxHei"] / 8;

    mg_tile.tile_set.Load("Assets/TileSet.png");
    bg_tile.tile_set.Load("Assets/TileSet.png");
    m2_tile.tile_set.Load("Assets/TileSet.png");

    sk_graphic::Texture2D temp;
    temp.Load("Assets/BackGrounds/sky.png");

    bg_sprite.LoadTexture(temp, glm::vec2(45, 25));

    std::cout << "Level Initialize done: " << level_name << '\n';
}
void Level::Load() {
    if (Loaded) return;
    std::cout << "Loading level: " << level_name << '\n';
    for (nlohmann::json layer : DATA["layerInstances"]) {
        std::cout << "loading layer:  " << layer["__identifier"] << "\n";
        if (layer["__identifier"] == "entities") {
            for (nlohmann::json jentity : layer["entityInstances"])
                LoadEntity(jentity);
            continue;
        }
        if (layer["__identifier"] == "main_tile_layer") {
            mg_tile.LoadLayer(layer, pos_topleft);
            continue;
        }
        if (layer["__identifier"] == "background_tile") {
            bg_tile.LoadLayer(layer, pos_topleft);
            continue;
        }
        if (layer["__identifier"] == "static_collider_layer") {
            m2_tile.LoadLayer(layer, pos_topleft);
            continue;
        }
        if (layer["__identifier"] == "static_colliders") {
            int width = layer["__cWid"];
            int height = layer["__cHei"];
            nlohmann::json collider_csv = layer["intGridCsv"];
            //static_collider_list.clear();
            for (int i = 0; i <= height - 1; i++) {
                for (int j = 0; j <= width - 1; j++) {
                    int csv_index = i * width + j;
                    if (collider_csv[csv_index] == 1) {
                        if (i == 0 || i == height - 1 || j == 0 || j == width - 1 || // check if is edge
                            collider_csv[csv_index - 1] != 1 ||                     // check if there is air next to it
                            collider_csv[csv_index + 1] != 1 ||
                            collider_csv[csv_index - width] != 1 ||
                            collider_csv[csv_index + width] != 1
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

    mg_tile.tiles.clear();
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
        case 7: //? trigger kill player that went out of play area
            AddTag(tag, etag::PHY_TRIGGER);
            AddTag(tag, etag::DAMAGE);
            static_collider_list.emplace_back(
            physic_world->Create_Body(sk_physic2d::Body_Def(
                sk_physic2d::irect::irect_fray(body_lowleft_pos, glm::vec2(1)),
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
    if (jentity["__identifier"] == "area_finish") {
        std::cout << "area finish ---------------------------------------------------";
        AreaFinish* e = new AreaFinish();
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

    for (auto e : m_entity) e->OnActive();
}
void Level::OnDeActive() {
    for (auto e : m_entity) e->OnDeActive();
}

void Level::Update() {
    for (auto e : m_entity) e->Update();

}
void Level::Draw() {
    m2_tile.Draw(0);
    mg_tile.Draw(0);
    bg_tile.Draw(-1);
    //spike_tile.Draw(0);

    glm::vec2 campos = sk_graphic::Renderer2D_GetCam()->position;
    bg_sprite.Draw(campos, -5, glm::vec2(.5f, .5f));
    for (Entity* e : m_entity) {
        e->Draw();
        if (e->CheckTag_(etag::SPAWN_POINT))
            sk_graphic::Renderer2D_AddDotX(glm::vec3((((PlayerSpawn*)e)->spawn_point), 1), { 1,1,0,1 });
    }
}
void Level::LateUpdate() {}