#ifndef __FOLLOW_H__
#define __FOLLOW_H__

#include "aecs/Entity.h"
#include "math/Vector2f.h"
#include <stdint.h>

namespace oge
{


struct Follow
{
    aecs::Entity entity = aecs::Entity(SIZE_MAX, SIZE_MAX);
    float speed = 1.f;
    Vector2f off = Vector2f(0.f, 0.f);
};


} // namespace oge
#endif // __FOLLOW_H__