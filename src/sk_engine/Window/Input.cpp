#include <iostream>

#include "Input.h"

#define bit(i,j) ((i>>j)&1)
namespace sk_input {
    namespace { //private section
        const Uint8* SDL_keystate; //= SDL_GetKeyboardState(NULL);

        const uint32_t key_range = 100;
        // sk engine only accept key that have sdl scancode value in [0 - 100]
        /*
            4   -   29      :       A   -   Z
            30  -   39      :       0   -   9
            58  -   69      :       F1  -   F12
            84  -   99      :       key pad
        */
        Uint8 SK_keystate[key_range];
        /*
            SDL_keystate only contain 0 and 1
            SK _keystate have 4 state
            2 bit
            first  bit: current frame key state
            second bit: previous frame key state

            01  1   key is     pressed this frame and NOT previous frame
            11  3   key is     pressed this frame and     previous frame
            00  0   key is NOT pressed this frame and NOT previous frame
            10  2   key is NOT pressed this frame and     pressed previous frame

            -------- 0 key is not pressed
            -------- 1 key is just pressed
            -------- 2 key is just released
            -------- 3 key is pressed and repeated
        */
        struct mouse_data {
            glm::ivec2  screen_pos;
            glm::vec3 world_pos;
        }mouse;
    }
    void Init() {
        SDL_keystate = SDL_GetKeyboardState(NULL);
    }
    void ShutDown() {}
    void Process() {
        for (int i = 0;i <= key_range - 1; i++) {
            SK_keystate[i] <<= 1;               //? *=2
            SK_keystate[i] |= SDL_keystate[i];  //? += sdl keystate    
            SK_keystate[i] &= 3;                //? %=4
        }
        SDL_GetMouseState(&mouse.screen_pos.x, &mouse.screen_pos.y);

    }

    //* keyboard
    bool Key(const sk_key& key) {
        return (SK_keystate[(int)key] & 1) == 1;
    }
    bool KeyDown(const sk_key& key) {
        return SK_keystate[(int)key] == 1;
    }
    bool KeyUp(const sk_key& key) {
        return SK_keystate[(int)key] == 2;
    }

    //* mouse
    glm::vec2 MousePos() {
        return mouse.screen_pos;
    }
}
#undef bit(i,j)
