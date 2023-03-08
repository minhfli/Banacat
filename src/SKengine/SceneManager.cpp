#include "SceneManager.h"

namespace { //private data, only acessible in this file
    Scene* current_scene = nullptr;

}
void LoadScene(SceneState scene) {
    if (current_scene)
        current_scene->OnDestroy();
}

void Update(float delta_time) {
    if (current_scene)
        current_scene->Update(delta_time);
}
void Update_Late(float delta_time) {
    if (current_scene)
        current_scene->Update_Late(delta_time);
}
void Update_Fixed(float delta_time) {
    if (current_scene)
        current_scene->Update_Fixed(delta_time);
}
void Draw() {
    if (current_scene)
        current_scene->Draw();
}