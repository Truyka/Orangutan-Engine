#ifndef __ANIMATIONSYSTEM_H__
#define __ANIMATIONSYSTEM_H__

#include "components/Animation.h"

#include "SystemManager.h"
#include "Config.h"

namespace oge
{


class AnimationSystem : public oge::System
{
public:
    OGE_SPECIFY_CLASS_NAME("AnimationSystem")

    void onNonFixedUpdate(const float dt) override;
};


} // namespace oge
#endif // __ANIMATIONSYSTEM_H__