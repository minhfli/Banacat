#include "Spring.h"
#include "../Area.h"
#include "../Level.h"
#include <sk_engine/Physics/AABB.h>
#include <sk_engine/Common/sk_time.h>
#include <iostream>

//sk_graphic::Sprite2D spring_sprite;

void Spring::OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {
    m_area = area;
    m_level = level;

    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::SPRING);

    glm::vec2 lowleft =
        m_level->pos_topleft +
        glm::vec2((float)jentity["__grid"][0], -(float)jentity["__grid"][1]) +
        glm::vec2(0, -1);

    pos = lowleft;

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

    ani.Init("Assets/Entity/Spring/spring", true);
    if (CheckTag_(etag::DIR_U)) ani.SetLayer("up");
    if (CheckTag_(etag::DIR_R)) ani.SetLayer("right");
    if (CheckTag_(etag::DIR_L)) ani.SetLayer("left");

    ani.SetState("idle");
    //sk_graphic::Texture2D tex;
    //tex.Load("Assets/Entity/Spring/spring.png");
    //spring_sprite.LoadTexture(tex, glm::vec2(2), glm::vec4(0, 0, 16, 16));
}

void Spring::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}
void Spring::Update() {
    ani.SetFrame_byCurrentTick(false);
}
void Spring::Draw() {
    ani.Draw(pos, 2, glm::vec2(0));
}

void Spring::OnTrigger(uint64_t trigger_tag) {
    if (CheckTag(trigger_tag, etag::PLAYER)) {
        ani.SetState("active");
    }
}