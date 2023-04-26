#include "Spring.h"
#include "../Area.h"
#include "../Level.h"
#include <sk_engine/Physics/AABB.h>

void Spring::OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {
    m_area = area;
    m_level = level;

    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::SPRING);

    glm::vec2 lowleft =
        m_level->pos_topleft +
        glm::vec2((float)jentity["__grid"][0], -(float)jentity["__grid"][1]) +
        glm::vec2(0, -1);


    float width = 0.5f;
    glm::vec2 ray_o;
    glm::vec2 ray_d;
    for (nlohmann::json field : jentity["fieldInstances"]) {
        if (field["__identifier"] == "direction") {
            if (field["__value"] == "up") {
                AddTag_(etag::DIR_U);
                ray_o = lowleft;
                ray_d = { 2,width };
            }
            if (field["__value"] == "down") {// down  v
                AddTag_(etag::DIR_D);
                ray_o = lowleft + glm::vec2(0, 2);
                ray_d = { 2,-width };
            }
            if (field["__value"] == "left") { //left  <
                AddTag_(etag::DIR_L);
                ray_o = lowleft + glm::vec2(2, 0);
                ray_d = { -width,2 };
            }
            if (field["__value"] == "right") {// right  >
                AddTag_(etag::DIR_R);
                ray_o = lowleft;
                ray_d = { width,2 };
            }
        }
    }
    trigger_body = m_area->GetPhysicWorld()->Create_Body(sk_physic2d::Body_Def(
        sk_physic2d::irect::irect_fray(ray_o, ray_d),
        tag,
        this
    ));

}

void Spring::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}