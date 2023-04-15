#pragma once

#include "Level.h"

#include <vector>
#include <string>
#include <thread>

#include <sk_engine/Physics/AABB_World.h>
#include "Player.h"

class Area {
    public:
    Area() {}
    ~Area() {}

    std::thread thread_LOAD_UNLOAD_LEVEL;
    bool thread_LOAD_UNLOAD_LEVEL_done = true;

    std::string area_name = "test";
    inline std::string get_path() { return "Assets/World/Areas/" + area_name + ".ldtk"; }

    bool Loaded = false;
    bool in_level_transition = false;

    int grid_size;

    std::string start_level = "level_0";

    int level_counts = 0;

    int active_level_index = -1;
    Level* Active_level = nullptr;
    std::vector<Level> m_levels;
    std::vector<std::vector<int>> neighborlist; // neighbor list of a level

    sk_physic2d::AABB_World physic_world;

    Player m_player;

    void Init();
    void Destroy();

    void LoadLevels(std::vector<int> loadlist);
    void UnLoadLevels(std::vector<int> unloadlist);

    void SetTransition(int to_level);

    void Start();
    void Update();
    void Draw();
    void LateUpdate();
    void Stop();
};
