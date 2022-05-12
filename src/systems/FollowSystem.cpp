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

        /// TODO: Change this to not use lerp and Vector2f.length()
        if(trackedPos != nullptr && (trackedPos->pos - t.pos).length() > 0.01f)
        {
            t.pos = Math::lerp(t.pos, trackedPos->pos, follow.speed);
        }
    });
}


} // namespace oge