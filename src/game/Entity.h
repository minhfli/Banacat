#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

// entity tag, entity can have multiple tag
enum e_tag: uint64_t {

    // for physic
    PHY_SOLID,
    PHY_ACTOR,
    PHY_TRIGGER,
    PHY_ONE_WAY, //should be solid, only work in upward direction

    PHY_MOVEABLE, // only physic body with this tag will be query and updated, all actor is moveable

    GROUND,

    SPAWN_POINT,
    PLAYER,

    DASH_REFRESH,

    DAMAGE
};
inline bool CheckTag(int i, int tag) {
    return ((i >> tag) & 1) != 0;
}
inline uint64_t AddTag(int& i, int tag) {
    i += (1ll << tag);
}

class Area;
class Level;

class Entity {
    public:
    Entity() {}
    ~Entity() {}

    Area* m_area = nullptr;
    Level* m_level = nullptr;

    uint64_t tag = 0;
    /// @brief add tag to entity
    inline void AddTag_(e_tag t) {
        if (!CheckTag_(t))
            tag += (1ll << (int)t);
    }
    /// @brief check if entity have tag
    inline bool CheckTag_(e_tag t) {
        return ((tag >> t) & 1) != 0;
    }

    virtual void OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {}
    virtual void OnCreate(Area* area, Level* level) {}
    virtual void OnDestroy() {}

    virtual void OnActive() {}
    virtual void OnDeActive() {}

    virtual void OnTrigger(Entity* trigger) {}
    virtual void OnTrigger(uint64_t trigger_tag) {}

    virtual void Start() {}
    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void Draw() {}
    virtual void DebugDraw() {}

};