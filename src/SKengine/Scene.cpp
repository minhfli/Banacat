#include "Scene.h"


Scene::Scene() {}
Scene::~Scene() {}



void LoadLevel(std::string filepath) {
    return;
}
void Scene::OnActive() {
    LoadLevel("level_0.ldtkl");
}

void Scene::OnDeactive() {}



void Scene::Start() {}

void Scene::Update() {}

void Scene::Draw() {}

void Scene::FixedUpdate() {}

