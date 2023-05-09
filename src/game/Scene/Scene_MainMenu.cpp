#include "Scene_MainMenu.h"
#include <sk_engine/Graphics/Graphics.h>
#include <sk_engine/Window/Input.h>
#include "SceneManager.h"

struct Scene_MainMenu::Private_Data {
    sk_graphic::Sprite2D main_bg;
};
Scene_MainMenu::Scene_MainMenu() {
    sdata = new Private_Data();
}

Scene_MainMenu:: ~Scene_MainMenu() {
    delete sdata;
}

void Scene_MainMenu::HintStr(int type, std::string hint) {}
void Scene_MainMenu::HintFlt(int type, float hint) {}
void Scene_MainMenu::HintInt(int type, int hint) {}

void Scene_MainMenu::OnCreate() {
    sk_graphic::Renderer2D_GetCam()->ProjectionO(720 / 2, 1280, 720);
    sk_graphic::Renderer2D_GetCam()->position = glm::vec3(1280 / 2, 720 / 2, 0);
    sk_graphic::Renderer2D_GetCam()->focus.pos = glm::vec2(1280 / 2, 720 / 2);
    sk_graphic::Renderer2D_GetCam()->focus.cambound = glm::vec4(-10000, -10000, 10000, 10000);

    sk_graphic::Texture2D tex;
    tex.Load("Assets/MainMenu/mainmenu.png");
    sdata->main_bg.LoadTexture(tex, glm::vec2(1280, 720));
}

void Scene_MainMenu::OnDestroy() {}

void Scene_MainMenu::Start() {}

void Scene_MainMenu::Stop() {}

void Scene_MainMenu::Update() {
    if (sk_input::KeyDown(sk_key::J)) SceneManager::LoadScene(Scene::GAMEAREA_TUTORIAL);
}

void Scene_MainMenu::Draw() {
    sdata->main_bg.Draw(glm::vec2(0), 0, glm::vec2(0));
}

void Scene_MainMenu::LateUpdate() {}
