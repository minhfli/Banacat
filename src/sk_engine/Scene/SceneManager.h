#pragma once

#include "SceneList.h"
#include "Scene.h"

namespace sk_scene_manager {
    void Setup();
    void LoadScene(SceneState scene, int special_number);

    void UpdateN(const int delta_tick, const float delta_time);
    void UpdateL(const int delta_tick, const float delta_time);
    void UpdateF(const int delta_tick, const float delta_time);
    void Draw();
} // namespace sk_scene_manager

