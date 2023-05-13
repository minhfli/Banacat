#include "Scene_GameArea.h"

#include <iostream>
#include <game/Area.h>
#include <sk_engine/Graphics/Graphics.h>

struct Scene_GameArea::Private_Data {
    Area area;
};
Scene_GameArea::Scene_GameArea() {
    sdata = new Scene_GameArea::Private_Data();
}
Scene_GameArea::~Scene_GameArea() {
    std::cout << "scene destructor called\n";
    delete sdata;
}

void Scene_GameArea::HintStr(int type, std::string hint) {
    switch (type) {
        case WORLD_NAME______:
            sdata->area.hint.area_name = hint;
            break;
        case START_LEVEL_____:
            sdata->area.hint.start_level = hint;
            break;
        case BACK_GROUND_____:
            sdata->area.hint.background = hint;
            break;
        default:
            break;
    }
}
void Scene_GameArea::HintFlt(int type, float hint) {}
void Scene_GameArea::HintInt(int type, int hint) {
    switch (type) {
        case WORLD_INDEX_____:
            sdata->area.hint.world_index = hint;
            break;
        default:
            break;
    }
}

// load and unload scene
void Scene_GameArea::OnCreate() {
    sdata->area.Init();
    sk_graphic::Renderer2D_GetCam()->ProjectionO(11.25f, 1280, 720);
}
void Scene_GameArea::OnDestroy() {
    sdata->area.Destroy();
}

void Scene_GameArea::Start() {
    sdata->area.Start();
}
void Scene_GameArea::Stop() {
    sdata->area.Stop();
}

// game loop
void Scene_GameArea::Update() { sdata->area.Update(); }
void Scene_GameArea::Draw() { sdata->area.Draw(); }
void Scene_GameArea::LateUpdate() { sdata->area.LateUpdate(); }