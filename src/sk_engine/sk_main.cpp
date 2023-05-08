#include "sk_main.h"
#include <sk_game.h>

#include "Common/sk_time.h"

#include "Graphics/Camera.h"
#include "Graphics/2D_Renderer.h"

#include "Window/skWindow.h"
#include "Window/Input.h"

#include <STB/stb_image.h>

#include <GLM/glm.hpp>

namespace sk_main {
    int window_w = 1280, window_h = 720;
    int last_fixed_update_tick = -100;

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

        float gamespeed = 1;

        sk_time::delta_time = sk_time::fixed_delta_time;
        sk_time::delta_tick = sk_time::fixed_delta_tick;

        sk_game::Start();
        while (!sk_window::Should_close()) {

            //calculate deltatick and deltatime
            if (sk_input::Key(sk_key::L)) gamespeed = 0.5f;
            else gamespeed = 1;
            sk_time::current_real_tick = SDL_GetTicks() * gamespeed;
            sk_time::current_real_time = (float)sk_time::current_real_tick * 0.001;

            // all game update and draw is in fixed time step
            if (sk_time::current_real_tick - last_fixed_update_tick >= sk_time::fixed_delta_tick) {
                last_fixed_update_tick += sk_time::fixed_delta_tick;

                sk_time::current_tick += sk_time::fixed_delta_tick;
                sk_time::current_time = (float)sk_time::current_tick * 0.001;

                if (sk_time::current_real_tick - last_fixed_update_tick >= sk_time::fixed_delta_tick) {
                    std::cout << "Error: Skiped " << sk_time::current_real_tick - last_fixed_update_tick << " ticks" << '\n';
                    last_fixed_update_tick = sk_time::current_real_tick;
                }

                sk_window::Process_event();
                sk_window::Clear();

                sk_graphic::Renderer2D_Begin();
                sk_game::GameLoop();

                sk_graphic::Renderer2D_End();
                sk_window::Swapbuffer();
            }
        }
        sk_game::Stop();
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
}