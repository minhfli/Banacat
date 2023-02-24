#pragma once

#include <string>

#include "Time.h"
#include "Tilemap.h"

class Scene
{
    private:
        /* data */
    public:
    Scene();
    ~Scene();

    //?load all game object here
    void OnActive();

    //?unload all game object here
    void OnDeactive();

    void Start();
    void Update();
    void Draw();
    void FixedUpdate();

};
