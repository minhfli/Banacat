#include "Crumble_platform.h"
#include "AllEntity.h"
#include <iostream>

#include "../Level.h"
#include "../Area.h"

#include <sk_engine/Common/sk_time.h>

struct Crumble_Platform::PRIVATE_DATA {
    sk_physic2d::AABB_World* physic_world;
    int m_body_index;
    sk_physic2d::Body* m_body;
    Animation ani;

    bool is_active = true;
    bool is_crumbling = false;
    int cell_count;
    std::vector<int> cell_variant;

    glm::vec2 cell_pos; // center pos of the fist cell

    float intertact_time;
    float crumble_duration = 1;
    float unactive_duration = 3; // this also cover crumble duration part 

    void Set_Crumble() {
        if (is_crumbling) return;
        is_crumbling = true;
        intertact_time = sk_time::current_time;
        ani.SetState("crumble");
    }
    void Update() {
        if (is_active) {
            if (is_crumbling) {
                if (intertact_time + crumble_duration < sk_time::current_time) {
                    is_crumbling = false;
                    is_active = false;
                    ani.SetState("unactive");
                    m_body->is_active = false;
                }
            }
        }
        else {
            if (intertact_time + unactive_duration < sk_time::current_time) {
                if (!physic_world->BoxCast(m_body->RECT.bound, (1 << etag::PHY_ACTOR))) {
                    is_active = true;
                    m_body->is_active = true;
                    ani.SetState("active");
                    for (auto& cell : cell_variant) cell = rand() % 3;
                }
            }
        }
        ani.SetFrame_byCurrentTick(false);
    }
    void Draw() {
        for (int i = 0; i <= cell_count - 1; i++) {
            ani.Draw(cell_pos + glm::vec2(1, 0) * (float)i, 0, glm::vec2(.5f), cell_variant[i]);
        }
    }
};
Crumble_Platform::Crumble_Platform() {
    pdata = new PRIVATE_DATA();
}
Crumble_Platform::~Crumble_Platform() {
    delete pdata;
}
void Crumble_Platform::OnJsonCreate(Area* area, Level* level, nlohmann::json& jentity) {
    m_level = level;
    m_area = area;

    AddTag_(etag::PHY_SOLID);
    AddTag_(etag::PHY_MOVEABLE);
    AddTag_(etag::GROUND);
    AddTag_(etag::PLATFORM_CRUMBLE);

    glm::vec4 bound = j_getbound(jentity, m_level->grid_size, m_level->pos_topleft);
    pdata->m_body_index = m_area->GetPhysicWorld()->Create_Body(sk_physic2d::Body_Def(
        sk_physic2d::irect::irect_fbound(bound),
        tag,
        this
    ));
    pdata->m_body = m_area->GetPhysicWorld()->Get_Body(pdata->m_body_index);
    pdata->physic_world = m_area->GetPhysicWorld();

    pdata->cell_count = bound.z - bound.x;
    pdata->cell_pos = bound.xy() + glm::vec2(0.5f);

    pdata->cell_variant.assign(pdata->cell_count, 0);
    for (auto& cell : pdata->cell_variant) cell = rand() % 3;

    pdata->ani.Init("Assets/Entity/crumble_platform/crumble_platform", true);
    pdata->ani.SetState("active");
}

void Crumble_Platform::OnDestroy() {
    m_area->GetPhysicWorld()->Remove_Body(pdata->m_body_index);
}

void Crumble_Platform::Update() {
    pdata->Update();
}

void Crumble_Platform::Draw() {
    pdata->Draw();
}

void Crumble_Platform::OnRiding(Entity* entity) {
    if (entity->CheckTag_(etag::PLAYER)) {
        pdata->Set_Crumble();
    }
}
