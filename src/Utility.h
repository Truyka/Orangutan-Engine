#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "components/Script.h"
#include "components/Transform.h"
#include "components/Collider.h"
#include "components/Physics.h"

#include <functional>

namespace oge
{


class CollisionResolver : public Scriptable
{
public:
    CollisionResolver(std::function<bool(Entity, CollisionInfo)> check)
    {
        collidesWith_ = check;
    }

    void onCollisionEnter(CollisionInfo info) override
    {
        if(info.time >= 0.0 && collidesWith_(info.object, info))
        {
            Velocity* vel = entity_.try_get<Velocity>();
            if(vel == nullptr) return;

            vel->vel += info.normal * Math::abs(vel->vel) * (1.f - info.time);
        }
    }

    std::function<bool(Entity, CollisionInfo)> collidesWith_;
};


} // namespace oge
#endif // __UTILITY_H__