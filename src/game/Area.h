#pragma once

#include "Level.h"

#include <vector>
#include <string>
#include <thread>

#include <sk_engine/Physics/AABB_World.h>
#include "Entities/Player.h"

// Game area, set hint for game area before call Init()
class Area {
    public:
    Area() {}
    ~Area() {}

    struct HINT {
        /// @brief world index in the ldtk project file
        int world_index = -1;
        std::string area_name = "";
        std::string start_level = "";
        std::string background = "";

    } hint;

    std::thread thread_LOAD_UNLOAD_LEVEL;
    bool thread_LOAD_UNLOAD_LEVEL_done = true;

    std::string area_name = "test";
    const std::string LDTK_PROJECT_FILE = "Assets/World/Areas/test.ldtk";

    bool Loaded = false;
    bool in_level_transition = false;

    int grid_size;

    int level_counts = 0;

    int active_level_index = -1;
    Level* Active_level = nullptr;
    std::vector<Level> m_levels;
    std::vector<std::vector<int>> neighborlist; // neighbor list of a level
    std::vector<glm::ivec4> level_bound_list;

    float level_transition_time = 0.5f;
    float level_transition_start_time;

    // list of level that should be drawn when transition beetween old-new level
    std::vector<int> draw_in_transition;

    sk_physic2d::AABB_World physic_world;

    Player m_player;

    sk_graphic::Sprite2D background;

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

    Player* GetPlayer() { return &m_player; }
    sk_physic2d::AABB_World* GetPhysicWorld() { return &physic_world; }
};
