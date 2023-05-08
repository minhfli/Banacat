#pragma once

#include <sk_engine/Graphics/Graphics.h>
#include <vector>
#include <string>
#include <unordered_map>

class Animation {
    public:
    Animation() {}
    ~Animation() {}

    std::string png_path = ".png";
    std::string json_path = ".json";

    struct Frame { // all frame should have the same size both in world and texture coordinate
        std::string name; // only needed for mult
        sk_graphic::Sprite2D sprite;
        int duration;
    };
    struct State {
        std::string name;
        int start;
        int end;
        int frame_count() { return end - start + 1; }
        int duration = 0;
        bool specical = false; // if state use special variable to determine frame or just loop through each frame
    };
    struct Layer { // use for similar animation but have different state, such as player animation that has multiple color state 
        std::string name;
    };
    bool multilayer = false;
    sk_graphic::Texture2D texture;
    std::vector <Frame> m_frame;
    std::vector <State> m_state;
    std::vector <Layer> m_layer;

    int current_Layer = 0;
    int current_State = 0;
    int current_Frame = 0;
    int frame_count;
    bool flipx = false, flipy = false;

    void Init(std::string Asset_path, bool multi_layer = false, int pixel_grid_size = 8);
    void SetState(std::string name);
    void SetLayer(std::string name);

    unsigned int state_start_tick; // start tick of current state

    void SetFrame_byDuration(int x, bool loop = true);
    void SetFrame_byCurrentTick(bool loop = true);
    void SetFrame_byIndex(int x, bool loop = false);

    void Draw(glm::vec2 pos, float depth, glm::vec2 pivot);
};