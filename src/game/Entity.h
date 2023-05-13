#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

// entity tag, entity can have multiple tag
enum etag : uint64_t {

    // for physic
    PHY_SOLID,
    PHY_ACTOR,
    PHY_TRIGGER,
    PHY_ONE_WAY, //should be solid, only work in upward direction(for now)

    PHY_DIR_U, // for one way body, this can change how trigger work, dont use for entity 
    PHY_DIR_D,
    PHY_DIR_L,
    PHY_DIR_R,

    DIR_U, // for special entity that need direction tag
    DIR_D,
    DIR_L,
    DIR_R,

    PHY_MOVEABLE, // only physic body with this tag will be query and updated, all actor is moveable

    GROUND,

    // each entity should not share tag
    SPAWN_POINT,
    AREA_FINISH,
    PLAYER,
    DASH_CRYSTAL,
    SPRING,

    PLATFORM_CRUMBLE,

    LEVEL_TRANS_FORCE_UP, // push player upward when transtion to an upper level

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

namespace sk_physic2d {
    class Body;
}

class Entity {
    public:
    Entity() {}
    virtual ~Entity() {}

    Area* m_area = nullptr;
    Level* m_level = nullptr;

    uint64_t tag = 0;
    /// @brief add tag to entity
    inline void AddTag_(etag t) {
        if (!CheckTag_(t))
            tag += (1ll << (int)t);
    }
    /// @brief check if entity have tag
    inline bool CheckTag_(etag t) {
        return ((tag >> t) & 1) != 0;
    }

    virtual void OnJsonCreate(Area* area, Level* level, nlohmann::json jentity) {}
    virtual void OnCreate(Area* area, Level* level) {}
    virtual void OnDestroy() {}

    virtual void OnActive() {}
    virtual void OnDeActive() {}

    virtual void OnTrigger(Entity* trigger) {}
    virtual void OnTrigger(uint64_t trigger_tag) {}
    //special type of trigger, call if a moveable solid is having an actor riding on it or an actor riding on the moveable solid
    virtual void OnRiding(Entity* entity) {}
    // call when actor is overlap a solid
    virtual void OnSquish() {}

    virtual void Start() {}
    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void Draw() {}
    virtual void DebugDraw() {}

    // check if actor is riding a solid,  
    virtual bool IsRiding(sk_physic2d::Body* solid) { return false; }
};