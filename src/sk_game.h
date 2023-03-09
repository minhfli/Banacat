#pragma once
/*
    CHANGE GAME HARDCOED SETTINGS HERE

    ONLY ADD OR CHANGE FUNTION IF YOU KNOW WHAT YOU ARE DOING
*/
namespace sk_game {
    const int default_screen_width = 800;

    const int fixed_delta_tick = 50;

    void Init();

    void Start();

    void UpdateN(const int delta_tick, const float delta_time);
    void UpdateL(const int delta_tick, const float delta_time);
    void UpdateF(const int delta_tick, const float delta_time);

    void Draw();
}