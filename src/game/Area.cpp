#include "Area.h"

#include <iostream>

#include "sk_engine/Common/Common.h"

void Area::Init() {
    physic_world.Hint_WorldBound(glm::vec2(0), 512);
    physic_world.Init();

    std::string area_ldtk = get_path();
    nlohmann::json area_data = ReadJsonFile(area_ldtk);

    this->grid_size = area_data["defaultGridSize"];

    this->level_counts = area_data["levels"].size();
    this->m_levels.reserve(this->level_counts);
    this->neighborlist.reserve(this->level_counts);

    for (int i = 0; i <= level_counts - 1; i++) {
        m_levels.emplace_back();
        m_levels[i].area_name = this->area_name;
        m_levels[i].level_name = area_data["levels"][i]["identifier"];
        m_levels[i].m_area = this;

        m_levels[i].Init();

        glm::ivec4 level_bound = glm::ivec4(
            (int)area_data["levels"][i]["worldX"],
            -(int)area_data["levels"][i]["worldY"] - (int)area_data["levels"][i]["pxHei"],
            (int)area_data["levels"][i]["worldX"] + (int)area_data["levels"][i]["pxWid"],
            -(int)area_data["levels"][i]["worldY"]
        ) / grid_size * sk_physic2d::INTCOORD_PRECISION;

        sk_physic2d::Body_Def level_bound_def(level_bound, 2, 0);
        physic_world.Create_Body(level_bound_def);

        neighborlist[i].reserve(area_data["levels"][i]["__neighbours"].size());
        for (int j = 0; j < i; j++)
            for (nlohmann::json neighbor : area_data["levels"][i]["__neighbours"])
                if (neighbor["levelIid"] == m_levels[j].level_iid) {
                    neighborlist[i].emplace_back(j);
                    neighborlist[j].emplace_back(i);
                }
    }
}
void Area::Destroy() {
    for (Level& level : m_levels)
        if (level.Loaded) level.UnLoad();
    physic_world.Clear();
}
void Area::LoadLevels(const std::vector<int> loadlist) {
    for (int index : loadlist)
        m_levels[index].Load();
}
void Area::UnLoadLevels(const std::vector<int> unloadlist) {
    for (int index : unloadlist)
        m_levels[index].UnLoad();
}

void Area::SetTransition(int to_level) {
    if (active_level_index == to_level) return;

    Active_level->OnDeActive();
    active_level_index = to_level;
    Active_level = &m_levels[to_level];
    in_level_transition = true;
    Active_level->OnActive();

    // throw error if (un)load level thread is not finished
    if (!thread_LOAD_UNLOAD_LEVEL_done)
        std::cout << "NOT DONE (UN)LOAD levels when trying to call NEW THREAD\n";
    if (thread_LOAD_UNLOAD_LEVEL.joinable()) thread_LOAD_UNLOAD_LEVEL.join();

    std::vector<bool> should_load;
    should_load.assign(0, level_counts);

    in_level_transition = true;
    for (int new_level_neighbor_index : neighborlist[to_level])
        should_load[new_level_neighbor_index] = true;
    should_load[to_level] = true;

    std::vector<int> loadlist; /*    */loadlist.reserve(level_counts);
    std::vector<int> unloadlist; /*  */unloadlist.reserve(level_counts);
    for (int i = 0; i <= level_counts - 1; i++) {
        if (should_load[i] && !m_levels[i].Loaded) loadlist.emplace_back(i);
        if (!should_load[i] && m_levels[i].Loaded) unloadlist.emplace_back(i);
    }

    thread_LOAD_UNLOAD_LEVEL = std::thread(
            [&]() {
                thread_LOAD_UNLOAD_LEVEL_done = false;
                LoadLevels(loadlist);
                UnLoadLevels(unloadlist);
                thread_LOAD_UNLOAD_LEVEL_done = true;
            }
    );
}
void Area::Start() {
    // load starting level
    for (int i = 0; i <= level_counts; i++) {
        if (i == level_counts) FatalError("START LEVEL NOT FOUND");
        if (m_levels[i].level_name == start_level) {
            m_levels[i].Load();
            m_levels[i].OnActive();
            active_level_index = i;
            Active_level = &m_levels[i];

            thread_LOAD_UNLOAD_LEVEL = std::thread(
                [&]() {
                    thread_LOAD_UNLOAD_LEVEL_done = false;
                    LoadLevels(neighborlist[i]);
                    thread_LOAD_UNLOAD_LEVEL_done = true;
                }
            );
            break;
        }
    }
    // load player
    sk_physic2d::Body_Def player_bodydef(sk_physic2d::irect::irect_fbound({ 0, 0, 1, 1.5 }), 1);
    m_player.m_body = physic_world.Create_Body(player_bodydef);
    m_player.physic_world = &physic_world;
}
void Area::Update() {
    if (in_level_transition) in_level_transition = false; //! need implementation
    else {
        m_player.Update();
        Active_level->Update();
    }
}

void Area::Draw() {
    if (in_level_transition);
    else {
        m_player.Draw();
        Active_level->Draw();
    }
}

void Area::LateUpdate() {
    if (in_level_transition);
    else {
        m_player.LateUpdate();
        Active_level->LateUpdate();
    }
}
void Stop() {}