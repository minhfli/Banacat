#pragma once

#include <sk_engine/Graphics/Graphic2D.h>
#include <vector>
#include <string>

class Animation {
    public:
    Animation() {}
    ~Animation() {}

    std::string png_path = ".png";
    std::string json_path = ".json";

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
        int frame_count() { return end - start + 1; }
        int duration = 0;
        bool specical = false; // if state use special variable to determine frame or just loop through each frame

    };
    std::vector <State> m_state;

    int current_State = 0;
    int current_Frame = 0;

    void Init(std::string Asset_path);
    void SetState(std::string name);

    bool flipx = false, flipy = false;
    unsigned int state_start_tick; // start tick of current state

    void SetFrame_byDuration(int x);
    void SetFrame_byCurrentTick();
    void SetFrame_byIndex(int x, bool loop = false);

    void Draw(glm::vec2 pos, float depth, glm::vec2 pivot);
};