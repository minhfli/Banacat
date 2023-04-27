#include "Animation.h"

#include <nlohmann/json.hpp>
#include <iostream>

#include <sk_engine/Common/Common.h>
#include <sk_engine/Common/sk_time.h>

void Animation::Init(std::string Asset_path, bool multi_layer, int pixel_grid_size) {
    json_path = Asset_path + ".json";
    png_path = Asset_path + ".png";
    nlohmann::json ani_data = ReadJsonFile(json_path);

    texture.Load(png_path);

    // load frames -------------------------------------------------------------------------------------------------
    m_frame.assign(ani_data["frames"].size(), Frame());
    for (int i = 0; i <= m_frame.size() - 1; i++) {
        glm::ivec4 uv = glm::ivec4(
            (int)ani_data["frames"][i]["frame"]["x"],
            (int)ani_data["frames"][i]["frame"]["y"],
            (int)ani_data["frames"][i]["frame"]["x"] + (int)ani_data["frames"][i]["frame"]["w"],
            (int)ani_data["frames"][i]["frame"]["y"] + (int)ani_data["frames"][i]["frame"]["h"]
        );

        glm::vec2 size = glm::vec2(uv.z - uv.x, uv.w - uv.y) / (float)pixel_grid_size;
        m_frame[i].name = ani_data["frames"][i]["filename"];
        m_frame[i].sprite.LoadTexture(texture, size, uv);
        m_frame[i].duration = (int)ani_data["frames"][i]["duration"];
    }
    // load states -------------------------------------------------------------------------------------------------
    m_state.assign(ani_data["meta"]["frameTags"].size(), State());
    for (int i = 0; i <= m_state.size() - 1; i++) {
        m_state[i].name = ani_data["meta"]["frameTags"][i]["name"];
        m_state[i].start = ani_data["meta"]["frameTags"][i]["from"];
        m_state[i].end = ani_data["meta"]["frameTags"][i]["to"];
        for (int f = m_state[i].start; f <= m_state[i].end;f++)
            m_state[i].duration += m_frame[f].duration;
    }
    // load layers -------------------------------------------------------------------------------------------------
    multilayer = multi_layer;
    if (multi_layer) {
        m_layer.assign(ani_data["meta"]["layers"].size(), Layer());
        for (int i = 0; i <= m_layer.size() - 1; i++)
            m_layer[i].name = ani_data["meta"]["layers"][i]["name"];
    }

    if (multilayer) frame_count = m_frame.size() / m_layer.size();
    else frame_count = m_frame.size();
}
void Animation::Draw(glm::vec2 pos, float depth, glm::vec2 pivot) {
    if (!multilayer)
        m_frame[current_Frame].sprite.Draw(pos, depth, pivot, flipx, flipy);
    else {
        m_frame[current_Frame + frame_count * current_Layer].sprite.Draw(pos, depth, pivot, flipx, flipy);
    }
}
void Animation::SetLayer(std::string name) {
    for (int i = 0; i <= m_layer.size() - 1; i++)
        if (m_layer[i].name == name) {
            current_Layer = i;
            return;
        }
}
void Animation::SetState(std::string name) {
    for (int i = 0; i <= m_state.size() - 1; i++)
        if (m_state[i].name == name) {
            current_State = i;
            current_Frame = m_state[i].start;
            state_start_tick = sk_time::current_tick;
            return;
        }
}
void Animation::SetFrame_byDuration(int x, bool loop) {
    //std::cout << x << '\n';
    if (loop) x %= m_state[current_State].duration;
    if (x >= m_state[current_State].duration) current_Frame = m_state[current_State].end;
    else for (int f = m_state[current_State].start; f <= m_state[current_State].end; f++)
        if (x < m_frame[f].duration) {
            current_Frame = f;
            break;
        }
        else x -= m_frame[f].duration;
}
void Animation::SetFrame_byCurrentTick(bool loop) {
    SetFrame_byDuration(sk_time::current_tick - state_start_tick, loop);
}
void Animation::SetFrame_byIndex(int x, bool loop) {
    if (loop) x %= m_state[current_State].frame_count();
    current_Frame = m_state[current_State].start + x;

    if (current_Frame > m_state[current_State].end) current_Frame = m_state[current_State].end;
}