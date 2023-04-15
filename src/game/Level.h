#pragma once

#include <string>
#include <GLM/glm.hpp>
#include <nlohmann/json.hpp>
#include <sk_engine/Physics/AABB_World.h>

#include "Tilemap.h"

class Area;

class Level {
    public:
    glm::vec2 position;

    bool Loaded = false;
    bool Active = false;
    std::string area_name;
    std::string level_name;
    std::string level_iid;

    nlohmann::json DATA; // this is set when call Init, to keep level progression

    inline std::string get_path() { return "Assets/World/Areas/" + area_name + "/" + level_name + ".ldtkl"; }

    glm::vec4 camera_bound;

    Tilemap m_tilemap;
    // list of static collider in physic world, use for unload
    std::vector<int> static_collider_list;

    Area* m_area;
    sk_physic2d::AABB_World* physic_world;

    // initialize level, set some basic data 
    void Init();

    // actually load data
    void Load();
    void UnLoad();

    void OnActive();
    void OnDeActive();

    void Update();
    void Draw();
    void LateUpdate();
};