#include "sk_game.h"
#include <Scene/SceneManager.h>
/*
    WRITE ALL GAME LOGIC HERE
    all update and draw funtion is called in gameloop function in sk_engine/sk_main.cpp

    update funtion has delta_tick and delta_time(seconds)
    1000 tick = 1 second
    use delta_tick when you need true time(animation and stuff)
*/
namespace sk_game {
    enum class GameState {
        NONE,

        SPLASH_SCREEN,

        MENU_MAIN,
        MENU_OPTION,

        GAME_PLAY

    }game_state = GameState::NONE;
    void Init() {
    }

    void Start() {}

    //? normal update, call before draw
    void UpdateN() {

    }
    //? late update, call after draw
    void UpdateL() {

    }

    void Draw() {}


    //! NOT IMPLEMENTED 
    //? fixed update, called after every fixed amount of time(not implemented)
    void UpdateF() { }

}