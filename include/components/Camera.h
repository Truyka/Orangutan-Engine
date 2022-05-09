#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "aecs/Registry.h"
#include "math/OgeMath.h"
#include "Transform.h"
#include "OgeRandom.h"
#include "Graphics.h"

#include <float.h>

namespace oge
{


enum class CameraType
{
    Normal,
    Centered
};

struct Camera
{
    CameraType type = CameraType::Centered;
    Vector2f off = Vector2f(0.0, 0.0);

    float shakeDuration = 0;
    float shakeForce = 0;
    Vector2f savedOffset = Vector2f(0.0, 0.0);
    
    void shake(float force, float duration)
    {
        if(shakeDuration <= 0)
        {
            savedOffset = off;
        }
        shakeDuration = duration;
        shakeForce = force;
    }

    static aecs::Entity getCurrent(aecs::Registry& reg);
    static Vector2f getCurrentPosition(aecs::Registry& reg, float interp = 1.f);
    static Vector2f getCurrentCenter(aecs::Registry& reg, float interp = 1.f);
};

inline aecs::Entity Camera::getCurrent(aecs::Registry& reg)
{
    auto cameras = reg.view<Camera, Transform>();
    aecs::Entity ent = cameras.front();
    return ent.isValid() ? ent : aecs::Entity();
}

inline Vector2f Camera::getCurrentPosition(aecs::Registry& reg, float interp)
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
        finalCameraPos.x -= window.w / 2;
        finalCameraPos.y -= window.h / 2;
    }

    finalCameraPos += camera.off;

    return finalCameraPos; 
}

inline Vector2f Camera::getCurrentCenter(aecs::Registry& reg, float interp)
{
    Rect window = Graphics::instance().getWindowRect();

    Vector2f pos = Camera::getCurrentPosition(reg, interp);
    pos.x += window.w / 2;
    pos.y += window.h / 2;

    return pos;
}


} // namespace camera
#endif // __CAMERA_H__