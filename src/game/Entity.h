#pragma once

#include <cstdint>

// entity tag, entity can have multiple tag
enum e_tag: uint64_t {

    // for physic
    SOLID,
    ACTOR,
    TRIGGER,
    ONE_WAY, //shourld be solid
    MOVEABLE,

    PLAYER
};

namespace sk_physic2d { class Body; }

class Entity {
    public:
    Entity() {}
    ~Entity() {}

    bool enable_debug_draw = 1;

    uint64_t tag = 0;
    /// @brief add tag to entity
    inline void AddTag(e_tag t) {
        if (!CheckTag(t))
            tag += (1ll << (int)t);
    }
    /// @brief check if entity have tag
    inline bool CheckTag(e_tag t) {
        return (tag & (1ll << (int)t)) != 0;
    }

    sk_physic2d::Body* physic_body = nullptr;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}

    virtual void OnActive() {}
    virtual void OnDeActive() {}

    virtual void OnTrigger(Entity* trigger) {}

    virtual void Start() {}
    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void Draw() {}
    virtual void DebugDraw() {}

};