#include "Area.h"

#include <iostream>

#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Graphics/Camera.h>
#include <sk_engine/Graphics/CamFocus.h>

#include <sk_engine/Common/Common.h>
#include <sk_engine/Common/sk_time.h>
#include <sk_engine/Window/Input.h>

namespace {
    bool ldtk_data_loaded = false;
    nlohmann::json LDTK_PROJECT_DATA;
}
void Area::Init() {
    //* init physic world
    physic_world.Hint_WorldBound(glm::vec2(0), 512);
    physic_world.Init();
    physic_world.enable_debug_draw = false;

    //* init play area 
    if (!ldtk_data_loaded) {
        std::cout << "load main ldtk project data\n";
        LDTK_PROJECT_DATA = ReadJsonFile(LDTK_PROJECT_FILE);
    }
    nlohmann::json area_data;
    if (hint.world_index != -1)
        area_data = LDTK_PROJECT_DATA["worlds"][hint.world_index];
    else if (hint.area_name != "") {
        bool found = false;
        for (auto world : LDTK_PROJECT_DATA["worlds"])
            if ((std::string)world["identifier"] == area_name) {
                area_data = world;
                found = true;
                break;
            }
        if (!found) {
            Error("Cannot find Area: " + hint.area_name + ". Make sure you typed correctly");
            Error("Area not set, loading default world");
            area_data = LDTK_PROJECT_DATA["worlds"][0];
        }
        else std::cout << "Use hint.world_index for faster loading\n";
    }
    else {
        Error("Area not set, loading default world");
        area_data = LDTK_PROJECT_DATA["worlds"][0];
    }
    area_name = area_data["identifier"];
    std::cout << "Initializing area(world): " << area_name;

    this->grid_size = 8;

    this->level_counts = area_data["levels"].size();
    this->m_levels.assign(level_counts, Level());
    this->neighborlist.assign(level_counts, std::vector<int>());
    this->level_bound_list.assign(level_counts, glm::ivec4());

    for (int i = 0; i <= level_counts - 1; i++) {
        std::cout << "Calling init: level " << i << '\n';
        m_levels[i].DATA = area_data["levels"][i];
        m_levels[i].m_area = this;
        m_levels[i].Init();

        level_bound_list[i] = glm::ivec4(
            (int)area_data["levels"][i]["worldX"],
            -(int)area_data["levels"][i]["worldY"] - (int)area_data["levels"][i]["pxHei"],
            (int)area_data["levels"][i]["worldX"] + (int)area_data["levels"][i]["pxWid"],
            -(int)area_data["levels"][i]["worldY"]
        ) / grid_size * sk_physic2d::INTCOORD_PRECISION;

        neighborlist[i].reserve(area_data["levels"][i]["__neighbours"].size());
        for (int j = 0; j < i; j++)
            for (nlohmann::json neighbor : area_data["levels"][i]["__neighbours"])
                if (neighbor["levelIid"] == m_levels[j].level_iid) {
                    neighborlist[i].emplace_back(j);
                    neighborlist[j].emplace_back(i);
                }
    }

    if (hint.start_level == "") hint.start_level = m_levels[0].level_name;

    //* init background
    sk_graphic::Texture2D temp;
    if (hint.background == "") {
        std::cout << "area backgound not set";
        hint.background = "default";
    }
    temp.Load("Assets/BackGrounds/" + hint.background + ".png");
    background.LoadTexture(temp, glm::vec2(45, 25));

    std::cout << "Area Initialize done\n";
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

    // wait for thread to done
    std::cout << "New active level: " << m_levels[to_level].level_name << "\n";
    if (!thread_LOAD_UNLOAD_LEVEL_done)
        std::cout << "NOT DONE (UN)LOAD levels when trying to call NEW THREAD\n";
    if (thread_LOAD_UNLOAD_LEVEL.joinable()) thread_LOAD_UNLOAD_LEVEL.join();

    Active_level->OnDeActive();
    active_level_index = to_level;
    Active_level = &m_levels[to_level];
    in_level_transition = true;
    Active_level->OnActive();

    // set cam transition 
    sk_graphic::Camera* cam = sk_graphic::Renderer2D_GetCam();
    glm::vec2 current_cam_pos = { cam->position.x,cam->position.y };
    glm::vec2 new_cam_pos = cam->focus.GetTarget_pos(m_player.GetCameraTarget());
    level_transition_start_time = sk_time::current_time;
    cam->focus.SetTransition(current_cam_pos, new_cam_pos, level_transition_time);

    // loading new level
    std::vector<bool> should_load(level_counts, 0);

    in_level_transition = true;
    for (int new_level_neighbor_index : neighborlist[to_level])
        should_load[new_level_neighbor_index] = true;
    should_load[to_level] = true;

    std::vector<int> loadlist; /*    */loadlist.reserve(level_counts);
    std::vector<int> unloadlist; /*  */unloadlist.reserve(level_counts);
    draw_in_transition.clear();/*    */draw_in_transition.reserve(level_counts);
    for (int i = 0; i <= level_counts - 1; i++) {
        if (should_load[i] && !m_levels[i].Loaded) loadlist.emplace_back(i);
        if (!should_load[i] && m_levels[i].Loaded) unloadlist.emplace_back(i);
        if (should_load[i] && m_levels[i].Loaded) draw_in_transition.emplace_back(i);
    }
    std::cout << "load list:            \n"; for (int i : loadlist) std::cout << i; std::cout << '\n';
    std::cout << "unload list:          \n"; for (int i : unloadlist) std::cout << i; std::cout << '\n';
    std::cout << "transition draw list: \n"; for (int i : draw_in_transition) std::cout << i; std::cout << '\n';

    thread_LOAD_UNLOAD_LEVEL = std::thread(
            [=]() {
                thread_LOAD_UNLOAD_LEVEL_done = false;
                LoadLevels(loadlist);
                UnLoadLevels(unloadlist);
                thread_LOAD_UNLOAD_LEVEL_done = true;
                std::cout << "thread done\n";
            }
    );
}
void Area::Start() {
    // load starting level
    std::cout << "Play area: start";
    for (int i = 0; i <= level_counts; i++) {
        if (i == level_counts) FatalError("START LEVEL NOT FOUND");
        if (m_levels[i].level_name == hint.start_level) {
            m_levels[i].Load();
            m_levels[i].OnActive();
            active_level_index = i;
            Active_level = &m_levels[i];

            thread_LOAD_UNLOAD_LEVEL = std::thread(
                [=]() {
                    thread_LOAD_UNLOAD_LEVEL_done = false;
                    LoadLevels(neighborlist[i]);
                    thread_LOAD_UNLOAD_LEVEL_done = true;
                }
            );
            break;
        }
    }
    // load player
    m_player.OnCreate(this, Active_level);
    m_player.OnNewLevel();
    m_player.Start();
}
void Area::Update() {

    // enable/disable physic debug draw
    if (sk_input::KeyDown(sk_key::KP_7)) physic_world.enable_debug_draw = !physic_world.enable_debug_draw;

    // check if player change level
    glm::ivec4 player_bound = physic_world.Get_Body(m_player.m_body_index)->RECT.bound;
    if (!sk_physic2d::contain(level_bound_list[active_level_index], player_bound))
        for (int neighbor_index : neighborlist[active_level_index]) {
            if (sk_physic2d::contain(level_bound_list[neighbor_index], player_bound)) {
                SetTransition(neighbor_index);
                m_player.OnNewLevel();
                return;
            }
        }

    // normal update
    if (in_level_transition) {
        if (sk_time::current_time > level_transition_start_time + level_transition_time)
            in_level_transition = false;
    }

    if (!in_level_transition) {
        m_player.Update();
        Active_level->Update();
        physic_world.Update();

    }
}

void Area::Draw() {
    glm::vec2 campos = sk_graphic::Renderer2D_GetCam()->position;
    background.Draw(campos, -5, glm::vec2(.5f, .5f));

    if (in_level_transition) {
        for (int index : draw_in_transition) {
            m_levels[index].Draw();
        }
        m_player.Draw();
    }
    else {
        m_player.Draw();
        Active_level->Draw();
    }
    physic_world.Draw();
}

void Area::LateUpdate() {
    if (in_level_transition) {
    }
    else {
        m_player.LateUpdate();
        Active_level->LateUpdate();
    }
}

void Area::Stop() {
    if (thread_LOAD_UNLOAD_LEVEL.joinable()) thread_LOAD_UNLOAD_LEVEL.join();
    for (Level& level : m_levels) if (level.Loaded) level.UnLoad();

    m_player.OnDestroy();
    physic_world.Clear();

}