#include "Main_Game.h"

#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Camera.h"
#include "Graphics/2D_Renderer.h"

#include "Window/skWindow.h"

#include <STB/stb_image.h>

#include <GLM/glm.hpp>

namespace sk_main {
    int window_w = 800, window_h = 600;

    /*
    ? SDL_keystate only contain 0 and 1
    ? SK _keystate have 4 state
    ? -------- 0 key is not pressed
    ? -------- 1 key is pressed
    ? -------- 2 key is just pressed
    ? -------- 3 key is just released
    */
    const Uint8* SDL_keystate; //= SDL_GetKeyboardState(NULL); 
    Uint8 SK_keystate[100];

    Camera cam;//*

    void Init() {
        out();
        sk_window_init("SDL window", window_w, window_h);

        //? stbi will now read image from bottom to top
        //stbi_set_flip_vertically_on_load(1);

        sk_graphic::Renderer2D_Init();

        //? get the pointer to keyboard state
        SDL_keystate = SDL_GetKeyboardState(NULL);
    }

    void Run() {

        //cam.ProjectionP(60, window_w, window_h);
        cam.ProjectionO(10, window_w, window_h);
        cam.position = glm::vec3(0.0f, 0.0f, 0.0f);

        sk_graphic::Renderer2D_GetCam(&cam);

        GLuint current_tick = SDL_GetTicks(), delta_tick;

        while (!sk_window_should_close()) {
            sk_window_process_event();

            delta_tick = SDL_GetTicks() - current_tick;
            float fps = (float)1000 / delta_tick;
            current_tick += delta_tick;

            //glm::mat4 model2 = glm::rotate(model, glm::radians((float)current_tick / 10), glm::vec3());
            //shader.SetMatrix4("transform", model2, 1);

            Update(delta_tick);

            sk_window_clear();

            Draw();
            sk_graphic::Renderer2D_End();

            sk_window_swapbuffer();

            //! std::cout reduce fps so this is not the fps
            //std::cout << delta_tick << " " << fps << '\n';

            if (delta_tick < 10) SDL_Delay(10 - delta_tick);
        }

        sk_graphic::Renderer2D_ShutDown();
    }

    void Awake() {}

    void Start() {}

    void Update(const int delta_tick) {
        //? 1 tick = 1/1000 second
        //? -> deltatime = deltatick/1000;
        const float delta_time = (float)delta_tick / 1000;

        //std::cout << (int)SDL_keystate[SDL_SCANCODE_A];
        if (SDL_keystate[SDL_SCANCODE_A]) cam.position += glm::vec3(-1, 0, 0) * delta_time * 5.0f;
        if (SDL_keystate[SDL_SCANCODE_D]) cam.position += glm::vec3(1, 0, 0) * delta_time * 5.0f;
        if (SDL_keystate[SDL_SCANCODE_W]) cam.position += glm::vec3(0, 1, 0) * delta_time * 5.0f;
        if (SDL_keystate[SDL_SCANCODE_S]) cam.position += glm::vec3(0, -1, 0) * delta_time * 5.0f;
    }

    void Draw() {
        int n = 10;
        /*for (int i = -n;i <= n; i++)
            for (int j = -n;j <= n; j++) {
                if ((i + j) % 2 == 0)
                    sk_graphic::Renderer2D_AddQuad(glm::vec3(i, j, 0), glm::vec2(1));
                sk_graphic::Renderer2D_AddLine(glm::vec3(i, j, 1), glm::vec2(0.5f));

            }*/
        sk_graphic::Renderer2D_AddDotX(glm::vec3(0));
        sk_graphic::Renderer2D_AddDotO(glm::vec3(0));
        //sk_graphic::Renderer2D_AddLine(glm::vec3(0, 0, 1), glm::vec2(10));

        //sk_graphic::Renderer2D_AddQuad(glm::vec3(1), glm::vec2(30));
    }

    void Quit() {
        SDL_Delay(500);
        SDL_Quit();
    }
}