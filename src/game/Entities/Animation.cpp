#include "Animation.h"

#include <nlohmann/json.hpp>
#include <iostream>

#include <sk_engine/Common/Common.h>

void Animation::Init() {
    nlohmann::json ani_data = ReadJsonFile(bananacat_json_path);

    texture.Load(bananacat_png_path);

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
    std::cout << ani_data["meta"]["frameTags"].size();

    for (int i = 0; i <= m_state.size() - 1; i++) {
        m_state[i].name = ani_data["meta"]["frameTags"][i]["name"];
        m_state[i].start = ani_data["meta"]["frameTags"][i]["from"];
        m_state[i].end = ani_data["meta"]["frameTags"][i]["to"];
    }
}