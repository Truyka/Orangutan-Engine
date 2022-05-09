#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "math/OgeMath.h"

namespace oge
{


/**
 * @brief Stores an entity's position and is used
 * for rendering position
*/
struct Transform
{
    Transform(float x, float y) : pos(x, y), oldPos(x, y) {}
    Transform(Vector2f _pos) : pos(_pos), oldPos(_pos) {}

    void setPosition(const Vector2f& newPos)
    {
        pos = newPos;
        oldPos = newPos;
    }

    void setPosition(float x, float y)
    {
        setPosition(Vector2f(x, y));
    }


    Vector2f pos;
    Vector2f oldPos;
};


} // namespace oge
#endif // __TRANSFORM_H__