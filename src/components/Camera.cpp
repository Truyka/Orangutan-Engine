#include "Camera.h"
#include "aecs/Registry.h"
#include "math/OgeMath.h"
#include "Transform.h"
#include "OgeRandom.h"
#include "Graphics.h"

#include <float.h>

namespace oge
{

void Camera::shake(float force, float duration)
{
    if(shakeDuration <= 0)
    {
        savedOffset = off;
    }
    shakeDuration = duration;
    shakeForce = force;
}

aecs::Entity Camera::getCurrent(aecs::Registry& reg)
{
    auto cameras = reg.view<Camera, Transform>();
    aecs::Entity ent = cameras.front();
    return ent.isValid() ? ent : aecs::Entity();
}

Vector2f Camera::getCurrentPosition(aecs::Registry& reg, float interp)
{
    aecs::Entity ent = Camera::getCurrent(reg);
    if(!ent.isValid())
        return Vector2f(0.f, 0.f);

    Camera& camera = reg.get<Camera>(ent);
    Transform& cameraTran = reg.get<Transform>(ent);

    Vector2f finalCameraPos = cameraTran.pos;

    if(camera.shakeDuration > 0)
    {
        camera.off = camera.savedOffset;
        camera.shakeDuration -= 1.f/60.f;

        camera.off += RNG.rollDirection() * camera.shakeForce;
    }

    finalCameraPos = Math::lerp(cameraTran.oldPos, cameraTran.pos, interp);

    Rect window = Graphics::instance().getWindowRect();
    if(camera.type == CameraType::Centered)
    {
        finalCameraPos.x -= window.w / camera.zoom / 2.f;
        finalCameraPos.y -= window.h / camera.zoom / 2.f;
    }

    finalCameraPos += camera.off;

    return finalCameraPos; 
}

Vector2f Camera::getCurrentCenter(aecs::Registry& reg, float interp)
{
    Rect window = Graphics::instance().getWindowRect();

    Vector2f pos = Camera::getCurrentPosition(reg, interp);
    pos.x += window.w / 2;
    pos.y += window.h / 2;

    return pos;
}

    
} // namespace oge