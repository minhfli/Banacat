#include "Main_Game.h"

#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Camera.h"
#include "Graphics/2D_Renderer.h"
#include <STB/stb_image.h>

#include <GLM/glm.hpp>

namespace sk_engine {
    int window_w = 800, window_h = 600;

    SDL_Window* window = nullptr;

    void Init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not be initialized: " << SDL_GetError();
            FatalError("Stop");
        }
        else
            std::cout << "SDL video system is ready to go\n";

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

        window = SDL_CreateWindow("C++ SDL2 Window",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_w,
            window_h,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        SDL_GL_CreateContext(window);

        gladLoadGLLoader(SDL_GL_GetProcAddress);

        glViewport(0, 0, window_w, window_h);
        glClearColor(0.1, 0.2, 0.3, 1.0);

        glEnable(GL_DEPTH_TEST);

        //? opengl wont draw back side of trianggle
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        //? stbi will now read image from bottom to top
        stbi_set_flip_vertically_on_load(1);

        Renderer2D_Init();

    }

    void Run() {

        Shader shader;
        shader.ID = GetShaderID();

        Camera cam;
        //cam.ProjectionP(60, window_w, window_h);
        cam.ProjectionO(2, window_w, window_h);
        cam.position = glm::vec3(0.0f, 0.0f, 0.0f);
        cam.CamMatrix(shader);

        int lastime = 0, time = 0;

        bool gameIsRunning = true;
        while (gameIsRunning) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    gameIsRunning = false;

            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Renderer2D_BeginBatch();
            Update();
            Draw();
            Renderer2D_EndBatch();

            SDL_GL_SwapWindow(window);

            time = SDL_GetTicks();
            float fps = (float)1000 / (time - lastime);

            std::cout << time - lastime << " ";
            std::cout << fps << '\n';

            lastime = time;


        //SDL_Delay(500);
        //break;
        }

        Renderer2D_ShutDown();

    }

    void Awake() {}

    void Start() {}

    void Update() {}

    void Draw() {
        int n = 100;
        GLfloat dis = 0.25f;
        GLfloat size = 0.2f;
        for (int i = -n;i <= n; i++)
            for (int j = -n;j <= n; j++) {
                float r = (float)(i + n) / (2 * n);
                float g = (float)(j + n) / (2 * n);
                float b = 1.0f;
                if (i == 0 && j == 0)
                    Renderer2D_AddQuad(glm::vec2(dis * i, dis * j), glm::vec2(size), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                else
                    Renderer2D_AddQuad(glm::vec2(dis * i, dis * j), glm::vec2(size), glm::vec4(r, g, b, 1.0f));
            }
    }

    void Quit() {
        SDL_DestroyWindow(window);
        SDL_Delay(500);
        SDL_Quit();
    }
}