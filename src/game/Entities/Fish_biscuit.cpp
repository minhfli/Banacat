#include "Fish_biscuit.h"
#include "AllEntity.h"

#include "../Area.h"
#include "../Level.h"

#include <sk_engine/Common/sk_time.h>
#include <sk_engine/Audio/skAudio.h>


void Fish_biscuit::OnJsonCreate(Area* area, Level* level, nlohmann::json& jentity) {
    m_area = area;
    m_level = level;

    AddTag_(etag::PHY_TRIGGER);
    AddTag_(etag::FISH_BISCUIT);

    glm::vec4 bound = j_getbound(jentity, m_level->grid_size, m_level->pos_topleft);
    trigger_body = m_area->GetPhysicWorld()->Create_Body(sk_physic2d::Body_Def(
        sk_physic2d::irect::irect_fbound(bound),
        tag,
        this
    ));

    for (nlohmann::json& field : jentity["fieldInstances"]) {
        if (field["__identifier"] == "active")
            active_field_instace = &field;
    }
    m_body = m_area->GetPhysicWorld()->Get_Body(trigger_body);
    if ((*active_field_instace)["__value"] == true) m_body->is_active = true;
    else m_body->is_active = false;

    ani.Init("Assets/Entity/fish_biscuit/fish_biscuit");
    if (m_body->is_active) ani.SetState("active");
    else ani.SetState("collect");

    sfx_collect = sk_audio::LoadSound("Assets/Audio/fish_get.ogg");
}

void Fish_biscuit::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(trigger_body);
}

void Fish_biscuit::OnTrigger(uint64_t trigger_tag) {
    if (CheckTag(trigger_tag, etag::PLAYER)) {
        m_body->is_active = false;
        //(*active_field_instace)["__value"] = false; ?unkown crash
        un_active_tick = sk_time::current_tick;
        ani.SetState("collect");
        sk_audio::PlaySound(sfx_collect);
    }

}
void Fish_biscuit::Draw() {
    ani.SetFrame_byDuration(sk_time::current_tick - un_active_tick, false);
    ani.Draw(m_body->RECT.true_center(), 0, glm::vec2(0));
}

void Fish_biscuit::Update() {
}
