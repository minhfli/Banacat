#include "AllEntity.h"
#include "../Area.h"
#include "../Level.h"

#include <sk_engine/Graphics/2D_Renderer.h>
#include <sk_engine/Common/sk_time.h>
#include <iostream>

void DashRefresh::OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {
    m_area = area;
    m_level = level;
    pos =
        m_level->pos_topleft +
        glm::vec2((float)jentity["__grid"][0], -(float)jentity["__grid"][1]) +
        glm::vec2((float)jentity["__pivot"][0], -(float)jentity["__pivot"][1]);

    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::DASH_REFRESH);
    trigger_body = m_area->GetPhysicWorld()->Create_Body(sk_physic2d::Body_Def(
        sk_physic2d::irect::irect_fsize_fpos(glm::vec2(0.5f), pos),
        tag,
        this
    ));

    m_texture.Load("Assets/dash_refresh.png");
}
void DashRefresh::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}
void DashRefresh::Update() {
    if (!is_active)
        if (sk_time::current_time - last_triggered_time > cooldown_time) is_active = 1;

}
void DashRefresh::Draw() {
    //std::cout << "dash refresh draw\n";
    if (is_active)
        sk_graphic::Renderer2D_AddQuad(
            pos,
            glm::vec2(1),
            3,
            glm::vec4(0, 0, 8, 8),
            m_texture,
            glm::vec4(1)
        );

}