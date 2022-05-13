#include "systems/FollowSystem.h"

#include "Scene.h"
#include "components/Follow.h"
#include "components/Transform.h"
#include "math/OgeMath.h"

namespace oge
{


void FollowSystem::onUpdate(const float dt)
{
    auto view = scene_->view<Follow, Transform>();
    view.each([&](Follow& follow, Transform& t)
    {
        Transform* trackedPos = scene_->try_get<Transform>(follow.entity);

        /// TODO: Change this to not use lerp
        if(trackedPos != nullptr)
        {
            t.pos = Math::interpTo(t.pos, trackedPos->pos, follow.speed);
        }
    });
}


} // namespace oge