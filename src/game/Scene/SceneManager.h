#pragma once

#include "BaseScene.h"

enum class Scene {
    NONE,

    SPASH_SCENE,

    MAIN_MENU,

    OPTION_MENU,

    GAMEAREA_TEST,
    GAMEAREA,
};
namespace SceneManager {

    BaseScene* GetActiveScene();

    //scene wont be loaded till the end of game loop, undefine behavior if called multiple time, each scene is indepentdant, cant pass data between scene
    void LoadScene(Scene toload);

    // call at the begining and ending of a gameloop
    void OnBeginLoop();
    void OnEndLoop();

    void Update();
    void Draw();
    void LateUpdate();
}