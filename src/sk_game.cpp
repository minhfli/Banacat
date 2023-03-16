#include "sk_game.h"
#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Physics/Test.h>
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
        cam->ProjectionO(10, 800, 600);
        cam->position = glm::vec3(0.0f, 0.0f, 0.0f);

    }

    void Start() {
        sk_physic2d::Setup();
        test_level::LoadLevel();
    }

    //? normal update, call before draw
    void UpdateN() {
        //std::cout << (int)SDL_keystate[SDL_SCANCODE_A];
        if (sk_input::Key(sk_key::A)) cam->position += glm::vec3(-1, 0, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::D)) cam->position += glm::vec3(1, 0, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::W)) cam->position += glm::vec3(0, 1, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::S)) cam->position += glm::vec3(0, -1, 0) * sk_time::delta_time * 5.0f;

        //! skphysic test.cpp
        sk_physic2d::Update(sk_time::delta_tick, *cam);
    }
    //? late update, call after draw
    void UpdateL() {}

    void Draw() {
        sk_physic2d::Draw();

        glm::vec3 mouse_world_pos = cam->Screen_To_World(sk_input::MousePos(), glm::vec2(800, 600));
        sk_graphic::Renderer2D_AddDotX(mouse_world_pos);

        test_level::Draw();

        for (int i = -10; i <= 10; i++)
            for (int j = -10; j <= 10; j++)
                if ((i + j) % 2 == 0)
                    sk_graphic::Renderer2D_AddQuad(
                        glm::vec3(i, j, -1),
                        glm::vec2(1),
                        glm::ivec4(0, 0, 16, 16));

    }


//! NOT IMPLEMENTED 
//? fixed update, called after every fixed amount of time(not implemented)
    void UpdateF() { }

    //? call when game stop, use to free data, save, ....
    void Stop() { }
}