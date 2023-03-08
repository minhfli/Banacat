#pragma once

#include <string>

#include "Tilemap.h"

class Scene {
    public:
    virtual void OnCreate() = 0; // Called when scene initially created. Called once per scene.
    virtual void OnDestroy() = 0; // Called when scene destroyed. Called once per scene.

    virtual void OnActivate() {}; // Called whenever a scene is activated.
    virtual void OnDeactivate() {}; // Called whenever a scene is deactivated.

    virtual void Update(const float& delta_time) {};
    virtual void Update_Late(const float& delta_time) {};
    virtual void Update_Fixed(const float& delta_time) {};
    virtual void Draw() {};
};
