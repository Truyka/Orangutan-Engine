#ifndef __MATH_H__
#define __MATH_H__

#include "Vector2f.h"
#include "Rect.h"
#include "Ray.h"

#include <cmath>

namespace oge::Math
{


inline float round(float x, int point)
{
    float pwd = std::pow(10, point);
    return std::round(x * pwd) / pwd;
}

inline float lerp(float v0, float v1, float t)
{
    return v1 * t + v0 * (1.0f - t);
}

inline Vector2f lerp(const Vector2f& v0, const Vector2f& v1, float t)
{
    return Vector2f(
        Math::lerp(v0.x, v1.x, t),
        Math::lerp(v0.y, v1.y, t)
    );
}

inline Vector2f abs(const Vector2f& vec)
{
    return Vector2f(std::abs(vec.x), std::abs(vec.y));
}

inline Vector2f min(const Vector2f& v0, const Vector2f& v1)
{
    return Vector2f(std::min(v0.x, v1.x), std::min(v0.y, v1.y));
}

inline Vector2f max(const Vector2f& v0, const Vector2f& v1)
{
    return Vector2f(std::max(v0.x, v1.x), std::max(v0.y, v1.y));
}

inline Vector2f round(const Vector2f& v)
{
    return v.rounded();
}

inline Vector2f normalize(const Vector2f& v)
{
    return v.normalized();
}

inline Rect unionRect(const Rect& r1, const Rect& r2)
{
    return r1.unionRect(r2);
}

inline float dot(const Vector2f& v1, const Vector2f& v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}


} // namespace oge
#endif // __MATH_H__