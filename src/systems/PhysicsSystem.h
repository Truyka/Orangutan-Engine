#ifndef __PHYSICSSYSTEM_H__
#define __PHYSICSSYSTEM_H__

#include "components/Transform.h"
#include "components/Physics.h"

#include "SystemManager.h"
#include "Config.h"

namespace oge
{


class PhysicsSystem : public System
{
public:
    OGE_SPECIFY_CLASS_NAME("PhysicsSystem")

    void onUpdate(const float dt) override;
};


} // namespace oge
#endif // __PHYSICSSYSTEM_H__