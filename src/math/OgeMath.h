#ifndef __SRC_MATH_OGEMATH__
#define __SRC_MATH_OGEMATH__

#include "Vector2f.h"
#include "Rect.h"
#include "Ray.h"

#include <cmath>

namespace oge
{


namespace Math
{


inline float round(float x, int point)
{
    float pwd = std::pow(10, point);
    return std::round(x * pwd) / pwd;
}

inline float lerp(float v0, float v1, float t)
{
    if(v0 == v1)
        return v1;
    return v1 * t + v0 * (1.0f - t);
}

inline Vector2f lerp(const Vector2f& v0, const Vector2f& v1, float t)
{
    return Vector2f(
        Math::lerp(v0.x, v1.x, t),
        Math::lerp(v0.y, v1.y, t)
    );
}

inline Vector2f interpTo(const Vector2f& v0, const Vector2f& v1, float speed)
{
    Vector2f diff = (v1 - v0);
    if(diff.length() < 0.5f)
    {
        return v1;
    }

    return Math::lerp(v0, v1, speed);
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

/**
 * @brief Provides a smooth transition from 0 to `lim` and
 * from `lim` to 0 as x grows. For example, it grows for
 * range <0, lim> and shrinks in range <lim, 2lim>, then it
 * grows again etc.
*/
inline float smlimit(float x, float lim)
{
    return std::abs(std::abs(std::fmod(x + lim, 2 * lim)) - lim);
}


} // namespace Math


} // namespace oge
#endif /* __SRC_MATH_OGEMATH__ */
