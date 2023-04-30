#pragma once

#include <string>

class BaseScene {

    public:
    // basic setup of new scene, call before scene create
    virtual void HintString(int type, std::string hint) {}
    virtual void HintFloat(int type, float hint) {}
    virtual void HintInt(int type, int hint) {}

    // load and unload scene
    virtual void OnCreate() {}
    virtual void OnDestroy() {}

    virtual void OnActive() {}
    virtual void OnDeActive() {}

    // game loop
    virtual void Update() {}
    virtual void Draw() {}
    virtual void LateUpdate() {}
};