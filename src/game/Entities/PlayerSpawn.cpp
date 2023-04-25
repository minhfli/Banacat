#include "PlayerSpawn.h"

#include "../Area.h"
#include "../Level.h"

void PlayerSpawn::OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {
    m_area = area;
    m_level = level;

    //physic_world = area->GetPhysicWorld();
    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::SPAWN_POINT);
    for (nlohmann::json field : jentity["fieldInstances"]) {
        if (field["__identifier"] == "Point") {
            spawn_point =
                m_level->pos_topleft +
                glm::vec2((float)field["__value"]["cx"], -(float)field["__value"]["cy"] - 1);
            continue;
        }
        if (field["__identifier"] == "Default" && field["__value"] == true) {
            m_level->special_level_data.default_spawn_point = spawn_point;
            continue;
        }
    }
    glm::vec4 bound =
        glm::vec4(m_level->pos_topleft, m_level->pos_topleft) +
        glm::vec4(
            (int)jentity["px"][0],
            -(int)jentity["px"][1] - (int)jentity["height"],
            (int)jentity["px"][0] + (int)jentity["width"],
            -(int)jentity["px"][1]
        ) / (float)m_level->grid_size;
    sk_physic2d::Body_Def player_spawn_bodydef(sk_physic2d::irect::irect_fbound(bound), tag, this);
    trigger_body = m_area->GetPhysicWorld()->Create_Body(player_spawn_bodydef);
}

void PlayerSpawn::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}