#include "AreaFinish.h"

#include "AllEntity.h"
#include "PlayerSpawn.h"

#include <sk_engine/Graphics/Graphics.h>
#include <game/Scene/SceneManager.h>
#include <iostream>

#include "../Area.h"
#include "../Level.h"

void AreaFinish::OnJsonCreate(Area* area, Level* level, nlohmann::json& jentity) {
    m_area = area;
    m_level = level;

    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::AREA_FINISH);

    std::string next_scene;
    for (nlohmann::json field : jentity["fieldInstances"]) {
        if (field["__identifier"] == "NextScene") {
            next_scene = field["__value"];
        }
    }
    glm::vec4 bound = j_getbound(jentity, m_level->grid_size, m_level->pos_topleft);

    sk_physic2d::Body_Def player_spawn_bodydef(sk_physic2d::irect::irect_fbound(bound), tag, this);
    trigger_body = m_area->GetPhysicWorld()->Create_Body(player_spawn_bodydef);
    std::cout << "trigger body: " << trigger_body << '\n';

    if (next_scene == ""                 /**/) scene_to_load = Scene::MAIN_MENU;
    if (next_scene == "menu"             /**/) scene_to_load = Scene::MAIN_MENU;
    if (next_scene == "next"             /**/) scene_to_load = (Scene)(SceneManager::GetCurrentSceneType() + 1); //! dangerous
    if (next_scene == "abandoned city"   /**/) scene_to_load = Scene::GAMEAREA_ABANDONED_CITY;
    if (next_scene == "the garden"       /**/) scene_to_load = Scene::GAMEAREA_THE_GARDEN;
}

void AreaFinish::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}
void AreaFinish::Draw() {
    auto body = m_area->GetPhysicWorld()->Get_Body(trigger_body);
    sk_graphic::Renderer2D_AddDotX({ body->RECT.bound.x,body->RECT.bound.y,1 });
}
void AreaFinish::OnTrigger(Entity* trigger) {}
void AreaFinish::OnTrigger(uint64_t trigger_tag) {
    if (CheckTag(trigger_tag, etag::PLAYER)) SceneManager::LoadScene(scene_to_load);
}