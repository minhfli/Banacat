#include <iostream>

#include "SceneManager.h"
#include "AllScene.h"

#include <sk_engine/Window/Input.h>

namespace SceneManager {

    BaseScene* ActiveScene = nullptr;
    BaseScene* GetActiveScene() { return ActiveScene; }

    bool new_scene_loaded = false;
    Scene scene_to_load = Scene::NONE;
    Scene current_scene = Scene::NONE;
    int GetCurrentSceneType() { return (int)current_scene; }

    void StartScene(BaseScene* scene) {
        if (scene == nullptr) return;
        scene->OnCreate();
        scene->Start();
    }
    void StopScene(BaseScene* scene) {
        if (scene == nullptr) return;
        scene->Stop();
        scene->OnDestroy();
    }
    // scene wont be loaded till the end of game loop only be loaded, undefine behavior  
    void LoadScene(Scene toload) {
        scene_to_load = toload;
    }
    // actually load new scene here
    void SetLoadScene() {
        StopScene(ActiveScene);
        if (ActiveScene) delete ActiveScene;

        switch (scene_to_load) {
            case Scene::MAIN_MENU:
                ActiveScene = new Scene_MainMenu();
                break;
            case Scene::GAMEAREA_TEST:
                ActiveScene = new Scene_GameArea();
                ActiveScene->HintInt(WORLD_INDEX_____, 0);
                ActiveScene->HintStr(START_LEVEL_____, "level_0");
                break;
            case Scene::GAMEAREA_TUTORIAL:
                ActiveScene = new Scene_GameArea();
                ActiveScene->HintInt(WORLD_INDEX_____, 0);
                ActiveScene->HintStr(START_LEVEL_____, "level_4");
                break;
            case Scene::GAMEAREA_ABANDONED_CITY:
                ActiveScene = new Scene_GameArea();
                ActiveScene->HintInt(WORLD_INDEX_____, 1);
                ActiveScene->HintStr(START_LEVEL_____, "level_5");
                break;
            default: // if scene not defined, load main menu
                //ActiveScene = nullptr;
                scene_to_load = Scene::MAIN_MENU;
                ActiveScene = new Scene_MainMenu();
                break;
        }
        current_scene = scene_to_load;
        scene_to_load = Scene::NONE;

        StartScene(ActiveScene);
    }

    void OnBeginLoop() {}
    void OnEndLoop() {
        //! dangerous, for fast changing between level only
        if (sk_input::KeyDown(sk_key::M)) scene_to_load = (Scene)((int)current_scene + 1);
        if (sk_input::KeyDown(sk_key::N)) scene_to_load = (Scene)((int)current_scene - 1);
        if (scene_to_load != Scene::NONE) SetLoadScene();
    }

    void Update() {
        if (ActiveScene) ActiveScene->Update();
    }
    void Draw() {
        if (ActiveScene) ActiveScene->Draw();
    }
    void LateUpdate() {
        if (ActiveScene) ActiveScene->LateUpdate();
    }

}