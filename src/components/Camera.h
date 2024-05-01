#ifndef __SRC_COMPONENTS_CAMERA__
#define __SRC_COMPONENTS_CAMERA__

#include "aecs/Entity.h"
#include "aecs/Registry.h"
#include "math/Vector2f.h"

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
    float zoom = 1.0;

    float shakeDuration = 0;
    float shakeForce = 0;
    Vector2f savedOffset = Vector2f(0.0, 0.0);
    
    void shake(float force, float duration);

    static aecs::Entity getCurrent(aecs::Registry& reg);
    static Vector2f getCurrentPosition(aecs::Registry& reg, float interp = 1.f);
    static Vector2f getCurrentCenter(aecs::Registry& reg, float interp = 1.f);
};


} // namespace camera
#endif /* __SRC_COMPONENTS_CAMERA__ */
