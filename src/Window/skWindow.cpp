#include <iostream>

#include <SDL2/SDL.h>
#include <GLAD/glad.h>

#include <Common/Error.h>

#include "skWindow.h"

static SDL_Window* window = nullptr;
static bool window_should_close;


void sk_window_init(const std::string name, const int width, const int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL video system could not be initialized: " << SDL_GetError();
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
        width,
        height,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_MAXIMIZED
    );

    SDL_GL_CreateContext(window);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    glViewport(0, 0, width, height);
}
void sk_window_quit() {
    SDL_DestroyWindow(window);
}

void sk_window_process_event() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                window_should_close = true;
                break;
            default:
                break;
        }
    }
}
bool sk_window_should_close() {
    return window_should_close;
}
void sk_window_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void sk_window_swapbuffer() {
    SDL_GL_SwapWindow(window);
}

void sk_keystate_get();
void sk_keystate_process();