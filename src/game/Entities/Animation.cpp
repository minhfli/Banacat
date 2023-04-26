#include "Animation.h"

#include <nlohmann/json.hpp>
#include <iostream>

#include <sk_engine/Common/Common.h>
#include <sk_engine/Common/sk_time.h>

void Animation::Init(std::string Asset_path) {
    json_path = Asset_path + ".json";
    png_path = Asset_path + ".png";
    nlohmann::json ani_data = ReadJsonFile(json_path);

    texture.Load(png_path);

    frames.assign(ani_data["frames"].size(), Frame());
    for (int i = 0; i <= frames.size() - 1; i++) {
        glm::ivec4 uv = glm::ivec4(
            (int)ani_data["frames"][i]["frame"]["x"],
            (int)ani_data["frames"][i]["frame"]["y"],
            (int)ani_data["frames"][i]["frame"]["x"] + (int)ani_data["frames"][i]["frame"]["w"],
            (int)ani_data["frames"][i]["frame"]["y"] + (int)ani_data["frames"][i]["frame"]["h"]
        );
        frames[i].sprite.LoadTexture(texture, glm::vec2(4), uv);
        frames[i].duration = (int)ani_data["frames"][i]["duration"];
    }
    m_state.assign(ani_data["meta"]["frameTags"].size(), State());

    for (int i = 0; i <= m_state.size() - 1; i++) {
        m_state[i].name = ani_data["meta"]["frameTags"][i]["name"];
        m_state[i].start = ani_data["meta"]["frameTags"][i]["from"];
        m_state[i].end = ani_data["meta"]["frameTags"][i]["to"];
        for (int f = m_state[i].start; f <= m_state[i].end;f++)
            m_state[i].duration += frames[f].duration;
    }
}
void Animation::Draw(glm::vec2 pos, float depth, glm::vec2 pivot) {
    frames[current_Frame].sprite.Draw(pos, depth, pivot, flipx, flipy);
}
void Animation::SetState(std::string name) {
    for (int i = 0; i <= m_state.size() - 1; i++)
        if (m_state[i].name == name) {
            current_State = i;
            current_Frame = m_state[i].start;
            //std::cout << "Animation state set: " << name << '\n';
            state_start_tick = sk_time::current_tick;
            break;
        }
}
void Animation::SetFrame_byDuration(int x) {
    //std::cout << x << '\n';
    x %= m_state[current_State].duration;
    for (int f = m_state[current_State].start; f <= m_state[current_State].end; f++)
        if (x < frames[f].duration) {
            current_Frame = f;
            break;
        }
        else x -= frames[f].duration;
}
void Animation::SetFrame_byCurrentTick() {
    SetFrame_byDuration((int)sk_time::current_tick - state_start_tick);
}
void Animation::SetFrame_byIndex(int x, bool loop) {
    if (loop) x %= m_state[current_State].frame_count();
    current_Frame = m_state[current_State].start + x;

    if (current_Frame > m_state[current_State].end) current_Frame = m_state[current_State].end;
}