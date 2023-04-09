#include "sk_game.h"
#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Window/Input.h>
#include <sk_engine/Common/sk_time.h>

#include <game/TestLevel.h>

/*
    WRITE ALL GAME LOGIC HERE
    all update and draw funtion is called in gameloop function in sk_engine/sk_main.cpp

    update funtion has delta_tick and delta_time(seconds)
    1000 tick = 1 second
    use delta_tick when you need true time(animation and stuff)


*/
namespace sk_game {
    namespace {
        Camera* cam;

        float camsize = 10;
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
        cam->ProjectionO(camsize, 800, 600);
        cam->position = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Start() {
        test_level::Start();
    }

    //! update cam size and positon, temporary
    void UpdateCam() {
        if (sk_input::Key(sk_key::KP_1)) cam->position += glm::vec3(-1, 0, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::KP_3)) cam->position += glm::vec3(1, 0, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::KP_5)) cam->position += glm::vec3(0, 1, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::KP_2)) cam->position += glm::vec3(0, -1, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::KP_4)) {
            camsize -= 10 * sk_time::delta_time;
            cam->ProjectionO(camsize, 800, 600);
        }
        if (sk_input::Key(sk_key::KP_6)) {
            camsize += 10 * sk_time::delta_time;
            cam->ProjectionO(camsize, 800, 600);
        }
    }
    //? normal update, call before draw
    void UpdateN() {
        //! update cam size and positon, temporary
        UpdateCam();

        test_level::Update();
    }
    //? late update, call after draw
    void UpdateL() {}

    void Draw() {
        glm::vec3 mouse_world_pos = cam->Screen_To_World(sk_input::MousePos(), glm::vec2(800, 600));
        sk_graphic::Renderer2D_AddDotX(mouse_world_pos);

        test_level::Draw();
    }

    //? fixed update, called after every fixed amount of time
    void UpdateF() { }

    //? call when game stop, use to free data, save, ....
    void Stop() {
        test_level::Stop();
    }
}