#include "sk_main.h"
#include <sk_game.h>

#include "Common/sk_time.h"

#include "Graphics/Camera.h"
#include "Graphics/2D_Renderer.h"

#include "Window/skWindow.h"
#include "Window/Input.h"

#include "Physics/Test.h"

#include <STB/stb_image.h>

#include <GLM/glm.hpp>

namespace sk_main {
    int window_w = 800, window_h = 600;

    Camera cam;//*

    void Init() {
        sk_window::Init("SDL window", window_w, window_h);

        //? stbi will now read image from bottom to top
        //stbi_set_flip_vertically_on_load(1);

        sk_graphic::Renderer2D_Init();

        sk_input::Init();

        sk_game::Init();
    }

    void Run() {
        //cam.ProjectionP(60, window_w, window_h);
        cam.ProjectionO(10, window_w, window_h);
        cam.position = glm::vec3(0.0f, 0.0f, 0.0f);

        sk_graphic::Renderer2D_GetCam(&cam);

        //! skphysic test.cpp
        sk_physic2d::Setup();

        sk_game::Start();
        while (!sk_window::Should_close()) {
            sk_window::Process_event();
            sk_window::Clear();

            sk_graphic::Renderer2D_Begin();

            //calculate deltatick and deltatime
            sk_time::delta_tick = SDL_GetTicks() - sk_time::current_tick;
            sk_time::delta_time = (float)sk_time::delta_tick / 1000;
            sk_time::current_tick += sk_time::delta_tick;
            sk_time::current_time = (float)sk_time::current_tick / 1000;
            std::cout << sk_time::delta_time << '\n';
            GameLoop();

            Update();
            Draw();

            sk_graphic::Renderer2D_End();
            sk_window::Swapbuffer();
        }
    }
    void Quit() {
        sk_graphic::Renderer2D_ShutDown();
        sk_window::ShutDown();
        sk_input::ShutDown();
        SDL_Delay(500);
        SDL_Quit();
    }

    void GameLoop() {
        sk_game::UpdateN();
        sk_game::Draw();
        sk_game::UpdateL();
    }


    void Update() {
        //std::cout << (int)SDL_keystate[SDL_SCANCODE_A];
        if (sk_input::Key(sk_key::A)) cam.position += glm::vec3(-1, 0, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::D)) cam.position += glm::vec3(1, 0, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::W)) cam.position += glm::vec3(0, 1, 0) * sk_time::delta_time * 5.0f;
        if (sk_input::Key(sk_key::S)) cam.position += glm::vec3(0, -1, 0) * sk_time::delta_time * 5.0f;

        //! skphysic test.cpp
        sk_physic2d::Update(sk_time::delta_tick, cam);
    }

    void Draw() {
        sk_physic2d::Draw();

        glm::vec3 mouse_world_pos = cam.Screen_To_World(sk_input::MousePos(), glm::vec2(800, 600));

        sk_graphic::Renderer2D_AddDotX(mouse_world_pos);

        /*int n = 10;
        for (int i = -n;i <= n; i++)
            for (int j = -n;j <= n; j++) {
                if ((i + j) % 2 == 0)
                    sk_graphic::Renderer2D_AddQuad(glm::vec3(i, j, -1), glm::vec2(1));
                sk_graphic::Renderer2D_AddLine(glm::vec3(i, j, 1), glm::vec2(0.5f));

            }*/
        //sk_graphic::Renderer2D_AddDotX(glm::vec3(0));
        //sk_graphic::Renderer2D_AddDotO(glm::vec3(0));
        //sk_graphic::Renderer2D_AddLine(glm::vec3(0, 0, 1), glm::vec2(10));

        //sk_graphic::Renderer2D_AddQuad(glm::vec3(1), glm::vec2(30));
    }

}