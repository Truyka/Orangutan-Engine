#ifndef __SRC_COMPONENTS_COLLIDER__
#define __SRC_COMPONENTS_COLLIDER__

#include "math/Rect.h"
#include "OgeEntity.h"
#include "Transform.h"

namespace oge
{


struct CollisionInfo
{
    Entity object;
    Vector2f normal = Vector2f(0.f, 0.f);
    float time = 0.f;
};

struct Collider
{
    Rect hitbox;
    bool skip  = false;
    bool added = false;
};

inline Rect computeHitbox(Transform& t, Collider& c)
{
    Vector2f& pos = t.pos;
    Rect& box = c.hitbox;

    return Rect(
        box.x + pos.x, 
        box.y + pos.y, 
        box.w * t.scale, 
        box.h * t.scale
    ).round();
}

inline Rect getHitbox(aecs::Registry& reg, aecs::Entity ent)
{
    Transform& t = reg.get<Transform>(ent);
    Collider& col = reg.get<Collider>(ent);

    return computeHitbox(t, col);
}

inline Rect getHitbox(oge::Entity ent)
{
    Transform& t = ent.get<Transform>();
    Collider& col = ent.get<Collider>();

    return computeHitbox(t, col);
}


} // namespace oge
#endif /* __SRC_COMPONENTS_COLLIDER__ */
