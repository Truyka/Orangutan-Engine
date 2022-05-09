#include "math/Ray.h"

#include <iostream>
#include <cmath>

namespace oge
{


Ray::Ray(Vector2f orig, Vector2f direction)
    : origin(orig), dir(direction), invdir(1.0 / dir)
{
    
}

Ray::Ray() : origin(0.0, 0.0), dir(0.0, 0.0), invdir(0.0, 0.0)
{

}

Ray Ray::fromPoints(Vector2f a, Vector2f b)
{
    Ray ray;
    ray.origin = a;
    ray.setDirection(b - a);
    return ray;
}

Ray Ray::fromOrigin(Vector2f orig, Vector2f direction)
{
    return Ray(orig, direction);
}

void Ray::setDirection(Vector2f d)
{
    dir = d;
    invdir = 1.0 / dir;
}

RaycastResult Ray::intersects(Rect r) const
{
    RaycastResult result{false, Vector2f(0.f, 0.f), Vector2f(0.f, 0.f), 0.f};

    Vector2f tmin = (r.getMin() - origin) * invdir;
    Vector2f tmax = (r.getMax() - origin) * invdir;

    if (std::isnan(tmax.y) || std::isnan(tmax.x)) return result;
    if (std::isnan(tmin.y) || std::isnan(tmin.x)) return result;

    if(tmin.x > tmax.x) std::swap(tmin.x, tmax.x);
    if(tmin.y > tmax.y) std::swap(tmin.y, tmax.y);

    if(tmin.x > tmax.y || tmin.y > tmax.x)
        return result;

    float hitmin = std::max(tmin.x, tmin.y);
    float hitmax = std::min(tmax.x, tmax.y);

    result.time = hitmin;

    if(hitmax < 0.0 || hitmin == hitmax)
    {
        return result;
    }

    result.point = origin + (dir * hitmin);
    if(tmin.x > tmin.y)
    {
        if(invdir.x < 0)
            result.normal = Vector2f(1, 0);
        else
            result.normal = Vector2f(-1, 0);
    }
    else
    {
        if(invdir.y < 0)
            result.normal = Vector2f(0, 1);
        else
            result.normal = Vector2f(0, -1);
    }
    
    result.hit = (hitmin < 1.0);
    return result;
}

RaycastResult Ray::sweepRect(Rect r1, Rect r2) const
{
    // To sweep an AABB across a ray, we need to inflate the second
    // AABB's hitbox by the swept AABB width and height
    Rect inflated = r2;
    inflated.x -= r1.w;
    inflated.y -= r1.h;
    inflated.w += r1.w;
    inflated.h += r1.h;

    return this->intersects(inflated);
}


} // namespace oge