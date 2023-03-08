#pragma once

#include "SceneList.h"
#include "Scene.h"

void LoadScene(SceneState scene);
void Setup();

void Update(const float& delta_time);
void Update_Late(const float& delta_time);
void Update_Fixed(const float& delta_time);
void Draw();