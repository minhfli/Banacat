#include "Test.h"
#include "AABB_World.h"

#include <Graphics/2D_Renderer.h>

#include <Common/Common.h>

namespace sk_physic2d {

    glm::vec3 mouse_pos;

    AABB_World physic_world;
    void Setup() {
        physic_world.Init();

        for (int i = 1; i <= 1000; i++) {
            rect R(
                glm::vec2(RandomFloat(-30, 30), RandomFloat(-30, 30)),
                glm::vec2(RandomFloat(0.5, 1), RandomFloat(0.5, 1))
            );
            Body_Def def(R);
            physic_world.Create_Body(def);
        }
    }
    void Update(uint32_t delta_time, Camera& cam) {
        mouse_pos = cam.Screen_To_World(sk_input::MousePos(), glm::vec2(800, 600));

        std::vector<int> query = physic_world.Query(rect(mouse_pos, glm::vec2(5)));

        if (sk_input::Key(sk_key::SPACE)) {
            for (int i : query) physic_world.Remove_Body(i);
            //physic_world.GetQuadTreeDebug();
        }
    }
    void Draw() {
        physic_world.Draw();

        //draw query rect
        rect query_rect = rect(mouse_pos, glm::vec2(5));
        sk_graphic::Renderer2D_AddBBox(query_rect.bound(), 1, glm::vec4(1, 1, 1, 1));

    }
}