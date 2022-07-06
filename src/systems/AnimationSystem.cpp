#include "AnimationSystem.h"

#include "components/Animation.h"
#include "components/Sprite.h"

#include "Scene.h"

namespace oge
{


void AnimationSystem::onNonFixedUpdate(const float dt)
{
    auto view = scene_->view<Animated, Sprite>();
    view.each([&](Animated& animated, Sprite& spr)
    {
        if(animated.size() == 0) return;

        Animation& anim = animated.current();
        Animation::Frame& frame = anim.current();
        spr.clip = frame.clip;
        spr.off  = frame.off;

        if(!anim.path.empty())
            spr.path = anim.path;

        anim.elapsed += dt;
        if(anim.elapsed >= anim.getInterval())
        {
            anim.elapsed -= anim.getInterval();

            if(anim.currentIdx + 1 < anim.framesCount())
            {
                anim.currentIdx++;
            }
            else if(anim.loop)
            {
                anim.currentIdx = 0;
            }
        }
    });
}


} // namespace oge