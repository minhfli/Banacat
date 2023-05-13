#include "AllEntity.h"
#include "../Area.h"
#include "../Level.h"

#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Common/sk_time.h>
#include <iostream>

void DashCrystal::OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {
    m_area = area;
    m_level = level;
    pos =
        m_level->pos_topleft +
        glm::vec2((float)jentity["__grid"][0], -(float)jentity["__grid"][1]) +
        glm::vec2((float)jentity["width"], -(float)jentity["height"]) / (float)m_area->grid_size / 2.0f;

    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::DASH_CRYSTAL);
    trigger_body = m_area->GetPhysicWorld()->Create_Body(sk_physic2d::Body_Def(
        sk_physic2d::irect::irect_fsize_fpos(glm::vec2(0.5f), pos),
        tag,
        this
    ));

    ani.Init("Assets/Entity/Dash_crystal/dash_crystal");

    ani.SetState("active");
}
void DashCrystal::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}

void DashCrystal::OnPlayerInteract() {
    is_active = false;
    last_triggered_time = sk_time::current_time;
    ani.SetState("unactive");
}
void DashCrystal::Update() {
    if (!is_active)
        if (sk_time::current_time - last_triggered_time > cooldown_time) {
            is_active = 1;
            ani.SetState("active");
        }
    ani.SetFrame_byCurrentTick();
}
void DashCrystal::Draw() {
    ani.Draw(pos, -.5f, glm::vec2(0.5f));
}