#include "sk_game.h"
#include <sk_engine/Graphics/Graphics.h>
#include <sk_engine/Window/Input.h>
#include <sk_engine/Common/sk_time.h>

#include <game/Area.h>

#include <iostream>
/*
    WRITE ALL GAME LOGIC HERE
    all update and draw funtion is called in gameloop function in sk_engine/sk_main.cpp

    update funtion has delta_tick and delta_time(seconds)
    1000 tick = 1 second
    use delta_tick when you need true time(animation and stuff)


*/
namespace sk_game {
    namespace {
        sk_graphic::Camera* cam;

        float camsize = 11.25f;

        Area GameArea;
    }
    enum class GameState {
        NONE,

        SPLASH_SCREEN,

        MENU_MAIN,
        MENU_OPTION,

        GAME_PLAY

    }game_state;

    void Init() {
        cam = sk_graphic::Renderer2D_GetCam();
        cam->ProjectionO(camsize, 1280, 720);
        cam->position = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Start() {
        //test_level::Start();
        GameArea.Init();
        GameArea.Start();
    }

    //! update cam size and positon, temporary
    void UpdateCam() {
        if (sk_input::Key(sk_key::KP_4)) {
            camsize -= 10 * sk_time::delta_time;
            cam->ProjectionO(camsize, 1280, 720);
        }
        if (sk_input::Key(sk_key::KP_6)) {
            camsize += 10 * sk_time::delta_time;
            cam->ProjectionO(camsize, 1280, 720);
        }
    }
    //? normal update, call before draw
    void UpdateN() {
        //! update cam size and positon, temporary
        UpdateCam();
        cam->Update();
        //test_level::Update();
        GameArea.Update();
    }
    //? late update, call after draw
    void UpdateL() {
        GameArea.LateUpdate();
    }

    void Draw() {
        glm::vec3 mouse_world_pos = cam->Screen_To_World(sk_input::MousePos(), glm::vec2(1280, 720));
        sk_graphic::Renderer2D_AddDotX(mouse_world_pos);

        cam->Draw();
        //test_level::Draw();
        GameArea.Draw();
    }

    //? fixed update, called after every fixed amount of time
    void UpdateF() { }

    //? call when game stop, use to free data, save, ....
    void Stop() {
        //test_level::Stop();
        GameArea.Stop();
        GameArea.Destroy();
    }
}