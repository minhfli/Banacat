#include "Dream_block.h"
#include "AllEntity.h"

#include "../Area.h"
#include "../Level.h"

#include "sk_engine/Graphics/Graphics.h"

void Dream_block::OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {
    m_area = area;
    m_level = level;

    AddTag_(etag::PHY_SOLID);
    AddTag_(etag::GROUND);
    AddTag_(etag::DREAM_BLOCK);
    for (nlohmann::json field : jentity["fieldInstances"]) {
    }

    glm::vec4 bound = j_getbound(jentity, m_level->grid_size, m_level->pos_topleft);
    sk_physic2d::Body_Def player_spawn_bodydef(sk_physic2d::irect::irect_fbound(bound), tag, this);
    trigger_body = m_area->GetPhysicWorld()->Create_Body(player_spawn_bodydef);
}

void Dream_block::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}

void Dream_block::Draw() {
    glm::vec4 bound = m_area->GetPhysicWorld()->Get_Body(trigger_body)->RECT.true_bound();
    sk_graphic::Renderer2D_AddBBox(bound, 0);

}
