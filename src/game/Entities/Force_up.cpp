#include "PlayerSpawn.h"
#include "AllEntity.h"

#include "../Area.h"
#include "../Level.h"

void Force_up::OnJsonCreate(Area* area, Level* level, nlohmann::json& jentity) {
    m_area = area;
    m_level = level;

    //physic_world = area->GetPhysicWorld();
    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::LEVEL_TRANS_FORCE_UP);
    for (nlohmann::json field : jentity["fieldInstances"]) {
    }

    glm::vec4 bound = j_getbound(jentity, m_level->grid_size, m_level->pos_topleft);
    sk_physic2d::Body_Def player_spawn_bodydef(sk_physic2d::irect::irect_fbound(bound), tag, this);
    trigger_body = m_area->GetPhysicWorld()->Create_Body(player_spawn_bodydef);
}

void Force_up::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}