#pragma once

#include <string>

class Scene {
    public:

    //use for create and destroy memory
    virtual void OnCreate() = 0; // Called when scene initially created. Called once per scene.
    virtual void OnDestroy() = 0; // Called when scene destroyed. Called once per scene.

    //hint for how to setup scene
    /*
        what?
            level number
            ...
    */
    virtual void SetHint(int special_number) {};

    //use for setup game logic
    virtual void OnActivate() {}; // Called whenever a scene is activated.
    virtual void OnDeactivate() {}; // Called whenever a scene is deactivated.

    virtual void UpdateN(const int delta_tick, const float delta_time) {};
    virtual void UpdateL(const int delta_tick, const float delta_time) {};
    virtual void UpdateF(const int delta_tick, const float delta_time) {};
    virtual void Draw() {};
};
