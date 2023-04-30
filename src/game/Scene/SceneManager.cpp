#include "SceneManager.h"

namespace SceneManager {

    BaseScene* ActiveScene = nullptr;
    BaseScene* GetActiveScene() { return ActiveScene; }

    Scene new_scene_to_load = Scene::NONE;

    // scene wont be loaded till the end of game loop only be loaded, undefine behavior  
    void LoadScene(Scene toload) {
        new_scene_to_load = toload;
    }
    // actually load new scene here
    void SetLoadScene() {}

    void OnBeginLoop() {}
    void OnEndLoop() {

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