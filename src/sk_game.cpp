#include "sk_game.h"
#include <Scene/SceneManager.h>
/*
    WRITE ALL GAME LOGIC HERE

    Dont think or care about fixed update, just dont
*/
namespace sk_game {
    void Init() {
        sk_scene_manager::Setup();

        //load splash scene when game start
        //sk_scene_manager::LoadScene(SceneState::SPLASH, 0);
    }

    void Start() {}

    //? normal update, call before draw
    void UpdateN(const int delta_tick, const float delta_time) {

    }
    //? late update, call after draw
    void UpdateL(const int delta_tick, const float delta_time) {

    }

    void Draw() { sk_scene_manager::Draw(); }

    //! SCREW THIS

    /// @brief fixed update, called after every fixed amount of time(not implemented)
    void UpdateF(const int delta_tick, const float delta_time) { sk_scene_manager::UpdateF(delta_tick, delta_time); }

}