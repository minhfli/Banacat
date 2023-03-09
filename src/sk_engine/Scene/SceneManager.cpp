#include "SceneManager.h"
#include <Physics/Test.h>
/*
    Scene manager

    right now all scene loading and logic is hardcoded
    change in the future
        ->  proper scene graph and loading
        ->  ecs and stuffs

    when new scene is loaded, old scene will be deleted so the app dont take memory when we have many scene
    'special data' (hard coded) could be transfer form old scene to new scene
*/
namespace sk_scene_manager {
    namespace { //private data, only acessible in this file
        // we cant delete a scene if that scene call scene manager to delete it self
        // so we need a pointer to delete that scene later 
        Scene* current_scene = nullptr;
        Scene* old_scene = nullptr;

        // scene state for transition
        SceneState current_state = SceneState::NONE;
        SceneState old_state = SceneState::NONE;

        // for future implement of trantition between scene
        bool inTransition = false;

        /*  skip the first frame(game loop) for every new scene
            mainly to keep delta_time low for the update function
            create new scene, delete old scene and do transition may take time, make the delta_time big
            so that's it, skip 1 gameloop then delta_time will be recalculated
        */
        int first_frame_skiped = -1; // equal 1 if skiped

        const float fixed_deltatime = 0.05; // 20 fixed update per second
    }

    void Setup() {}

    /// @brief 
    /// @param target target scene type
    /// @param special_number hint for scene manager to load the right scene
    void LoadScene(SceneState target, int special_number) {
        if (current_scene) {
            current_scene->OnDestroy();
            current_scene->OnDeactivate();
            old_scene = current_scene;
            old_state = current_state;
        }

        switch (target) {
            case SceneState::NONE:
                current_scene = nullptr;
                break;

        }
        if (current_scene) {
            current_scene->SetHint(special_number);
            current_scene->OnCreate();
            current_scene->OnActivate();
        }

    }

    void UpdateN(const int delta_tick, const float delta_time) {
        //delete old scene
        if (old_scene) {
            delete old_scene;
            old_scene = nullptr;
            old_state = SceneState::NONE;
        }

        if (current_scene)
            current_scene->UpdateN(delta_tick, delta_time);
    }
    void UpdateL(const int delta_tick, const float delta_time) {
        if (current_scene)
            current_scene->UpdateL(delta_tick, delta_time);
    }
    void UpdateF(const int delta_tick, const float delta_time) {
        if (current_scene)
            current_scene->UpdateF(delta_tick, delta_time);
    }
    void Draw() {
        if (current_scene)
            current_scene->Draw();
    }
} // namespace sk_scene_manager
