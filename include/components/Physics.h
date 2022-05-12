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

struct Damping
{
    Damping(float cx, float cy) : damping(cx, cy) {}
    Damping(float v) : damping(v, v) {}
    Damping(Vector2f d) : damping(d) {}

    Vector2f damping;
};

struct Acceleration
{
    Acceleration(float cx, float cy) : acc(cx, cy) {}
    Acceleration(Vector2f accel) : acc(accel) {}

    Vector2f acc;
};


} // namespace oge
#endif // __PHYSICS_H__