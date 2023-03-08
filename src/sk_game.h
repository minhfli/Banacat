#pragma once
/*
    CHANGE GAME HARDCOED SETTINGS HERE

    ONLY ADD OR CHANGE FUNTION IF YOU KNOW WHAT YOU ARE DOING
*/
namespace sk_game {
    const int default_screen_width = 800;


    void Init();

    void Start();

    void Update(const float& delta_time);
    void Update_Late(const float& delta_time);
    void Update_Fixed(const float& delta_time);

    void Draw();
}