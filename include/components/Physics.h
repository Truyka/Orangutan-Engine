#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "math/OgeMath.h"

namespace oge
{


struct Velocity
{
    Velocity(float cx, float cy) : vel(cx, cy) {}
    Velocity(Vector2f cpos) : vel(cpos) {}

    Vector2f vel;
};


} // namespace oge
#endif // __PHYSICS_H__