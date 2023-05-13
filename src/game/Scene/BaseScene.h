#pragma once

#include <string>

enum Hint_type {
    WORLD_INDEX_____,
    WORLD_NAME______,
    START_LEVEL_____,
    BACK_GROUND_____,

    HINT_NONE
};
class BaseScene {

    public:
    BaseScene() {}
    virtual ~BaseScene() {}

    // basic setup of new scene, call before scene create
    virtual void HintStr(int type, std::string hint) {}
    virtual void HintFlt(int type, float hint) {}
    virtual void HintInt(int type, int hint) {}

    // load and unload scene
    virtual void OnCreate() {}
    virtual void OnDestroy() {}

    virtual void Start() {}
    virtual void Stop() {}

    // game loop
    virtual void Update() {}
    virtual void Draw() {}
    virtual void LateUpdate() {}
};