#include "Main_Game.h"

#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Camera.h"

#include <STB/stb_image.h>

#include <GLM/glm.hpp>

namespace sk_engine {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0, 0,
        0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,    1, 0,
        0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,    1, 1,
        -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    0, 1,
    };
    GLuint indices[] = {
        0,1,2,
        0,2,3
    };
    int window_w = 800, window_h = 600;

    SDL_Window* window = nullptr;

    void Init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not be initialized: " <<
                SDL_GetError();
        }
        else
            std::cout << "SDL video system is ready to go\n";

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        window = SDL_CreateWindow("C++ SDL2 Window",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_w,
            window_h,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        SDL_GLContext context = SDL_GL_CreateContext(window);

        gladLoadGLLoader(SDL_GL_GetProcAddress);

        glViewport(0, 0, window_w, window_h);
        glClearColor(0.1, 0.2, 0.3, 1.0);

        glEnable(GL_DEPTH_TEST);

        stbi_set_flip_vertically_on_load(1);
    }

    void Run() {

        Camera cam;
        cam.ProjectionP(60, window_w, window_h);
        cam.position = glm::vec3(0.0f, 0.0f, 2.0f);
        //cam.CamMatrix(shader);

        glm::mat4 model = glm::mat4(1);

        int total_units;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &total_units);
        std::cout << total_units << '\n';  // the result is 192

                //! dadadsda
                //? DRAW IN WIREFRAME MODE
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                //? DRAW IN Fill MODE
                //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        bool gameIsRunning = true;

        while (gameIsRunning) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    gameIsRunning = false;

            }


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SDL_GL_SwapWindow(window);
        }

    }

    void Awake() {}

    void Start() {}

    void Update() {}

    void Draw() {}

    void Quit() {
        SDL_DestroyWindow(window);
        SDL_Delay(500);
        SDL_Quit();
    }
}