#pragma once

#include <string>

#include "Time.h"

class Scene
{
    private:
        /* data */
    public:
    Scene();
    ~Scene();

    void OnActive();
    void OnDeactive();

    void Start();
    void Update();
    void Draw();
    void FixedUpdate();

};
