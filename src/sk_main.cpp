#include "sk_main.h"
#include <sk_game.h>

#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture2D.h"
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

        GLuint current_tick = SDL_GetTicks(), delta_tick;

        sk_physic2d::Setup();
        while (!sk_window::Should_close()) {
            sk_window::Process_event();

            delta_tick = SDL_GetTicks() - current_tick;
            float fps = (float)1000 / delta_tick;
            current_tick += delta_tick;

            Update(delta_tick);

            sk_window::Clear();

            Draw();

            sk_window::Swapbuffer();

            //! std::cout reduce fps so this is not the actual fps
            //std::cout << delta_tick << " " << fps << '\n';
        }
    }

    void Awake() {}

    void Start() {}

    void Update(const int delta_tick) {
        //? 1 tick = 1/1000 second
        //? -> deltatime = deltatick/1000;
        const float delta_time = (float)delta_tick / 1000;

        //std::cout << (int)SDL_keystate[SDL_SCANCODE_A];
        if (sk_input::Key(sk_key::A)) cam.position += glm::vec3(-1, 0, 0) * delta_time * 5.0f;
        if (sk_input::Key(sk_key::D)) cam.position += glm::vec3(1, 0, 0) * delta_time * 5.0f;
        if (sk_input::Key(sk_key::W)) cam.position += glm::vec3(0, 1, 0) * delta_time * 5.0f;
        if (sk_input::Key(sk_key::S)) cam.position += glm::vec3(0, -1, 0) * delta_time * 5.0f;

        sk_physic2d::Update(delta_tick, cam);
    }

    void Draw() {
        sk_graphic::Renderer2D_Begin();
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
        sk_graphic::Renderer2D_End();
    }

    void Quit() {
        sk_graphic::Renderer2D_ShutDown();
        sk_window::ShutDown();
        sk_input::ShutDown();
        SDL_Delay(500);
        SDL_Quit();
    }
}