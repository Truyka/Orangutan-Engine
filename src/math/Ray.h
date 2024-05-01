#ifndef __RAY_H__
#define __RAY_H__

#include "Vector2f.h"
#include "Rect.h"

namespace oge
{


struct RaycastResult
{
    bool hit;
    Vector2f point;
    Vector2f normal;
    float time;

    operator bool()
    {
        return hit;
    }
};

class Ray
{
public:
    Ray(Vector2f orig, Vector2f direction);
    Ray();

    static Ray fromPoints(Vector2f a, Vector2f b);
    static Ray fromOrigin(Vector2f orig, Vector2f direction);

    void setDirection(Vector2f d);

    RaycastResult intersects(Rect r) const;
    RaycastResult sweepRect(Rect r1, Rect r2) const;

public:
    Vector2f origin;
    Vector2f dir;
    Vector2f invdir;
};


} // namespace oge
#endif // __RAY_H__