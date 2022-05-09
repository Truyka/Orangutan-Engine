#ifndef __FOLLOWSYSTEM_H__
#define __FOLLOWSYSTEM_H__

#include "components/Follow.h"

#include "SystemManager.h"
#include "Config.h"

namespace oge
{


class FollowSystem : public System
{
public:
    OGE_SPECIFY_CLASS_NAME("FollowSystem")

    void onUpdate(const float dt) override;
};


} // namespace oge
#endif // __FOLLOWSYSTEM_H__