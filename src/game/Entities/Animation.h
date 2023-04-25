#pragma once

#include <sk_engine/Graphics/Graphic2D.h>
#include <vector>
#include <string>

class Animation {
    public:
    Animation() {}
    ~Animation() {}

    const std::string bananacat_png_path = "Assets/bananacat.png";
    const std::string bananacat_json_path = "Assets/bananacat.json";

    struct Frame { // all frame should have the same size both in world and texture coordinate
        sk_graphic::Sprite2D sprite;
        int duration;
    };
    std::vector<Frame> frames;
    sk_graphic::Texture2D texture;

    struct State {
        std::string name;
        int start;
        int end;
        bool specical = false; // if state use special variable to determine frame or just loop through each frame
    };
    std::vector <State> m_state;
    State current;
    void Init();
};