#pragma once

#include "AABB.h"
#include <Graphics/Camera.h>
#include <Window/Input.h>
namespace sk_physic2d {
    void Setup();
    void Update(uint32_t delta_time, Camera& cam);
    void Draw();
}