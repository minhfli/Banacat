#include <iostream>

#include "SceneManager.h"
#include "AllScene.h"

namespace SceneManager {

    BaseScene* ActiveScene = nullptr;
    BaseScene* GetActiveScene() { return ActiveScene; }

    bool new_scene_loaded = false;
    Scene scene_to_load = Scene::NONE;


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
            case Scene::GAMEAREA_TEST:
                ActiveScene = new Scene_GameArea();
                break;
            case Scene::MAIN_MENU:
                ActiveScene = new Scene_MainMenu();
                break;
            default:
                ActiveScene = nullptr;
                break;
        }
        scene_to_load = Scene::NONE;

        StartScene(ActiveScene);
    }

    void OnBeginLoop() {}
    void OnEndLoop() {
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