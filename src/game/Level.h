#pragma once

#include <string>
#include <GLM/glm.hpp>
#include <nlohmann/json.hpp>
#include <sk_engine/Physics/AABB_World.h>

#include "Entity.h"

#include "Tilemap.h"

class Area;

class Level {
    public:
    // top left pos of level in world coord
    int grid_size;
    glm::vec2 pos_topleft;

    bool Loaded = false;
    bool Active = false;
    std::string area_name;
    std::string level_name;
    std::string level_iid;

    nlohmann::json DATA; // this is set when call Init, to keep level progression

    inline std::string get_path() { return "Assets/World/Areas/" + area_name + "/" + level_name + ".ldtkl"; }

    Tilemap m_tilemap;
    // list of static collider in physic world, use for unload
    std::vector<int> static_collider_list;

    Area* m_area;
    sk_physic2d::AABB_World* physic_world;

    struct SPECIAL_LEVEL_DATA {
        glm::vec4 camera_bound = glm::vec4(-1000, -1000, 1000, 1000);
        glm::vec2 default_spawn_point;
    } special_level_data;

    std::vector<Entity*> m_entity;

    // initialize level, set some basic data 
    void Init();

    // actually load data
    void Load();
    void UnLoad();

    void LoadStaticBody(int type, glm::vec2 lowleft_pos);
    void LoadEntity(nlohmann::json jentity);
    void UnLoadEntity();

    void OnActive();
    void OnDeActive();

    void Update();
    void Draw();
    void LateUpdate();
};