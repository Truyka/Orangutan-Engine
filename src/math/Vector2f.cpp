#include "math/Vector2f.h"
#include "math/OgeMath.h"

#include <cmath>
#include <float.h>

namespace oge
{

Vector2f Vector2f::null = Vector2f(FLT_MAX, FLT_MAX);

Vector2f::Vector2f(float val) : x(val), y(val)
{
    
}

Vector2f::Vector2f(float xc, float yc)
    : x(xc), y(yc)
{

}

Vector2f::Vector2f() : x(0.0), y(0.0)
{

}

std::string Vector2f::toString() const
{
    return "(" + std::to_string(Math::round(x, 2)) + "," + std::to_string(Math::round(y, 2)) + ")";
}

Vector2f Vector2f::fromDegrees(float degree)
{
    return Vector2f::fromRadians(degree * 0.0174533);
}

Vector2f Vector2f::fromRadians(float rad)
{
    return Vector2f(std::cos(rad), std::sin(rad));
}

float Vector2f::toDegrees()
{
    return this->toRadians() * 57.2957795;
}

float Vector2f::toRadians()
{
    return std::atan2(y, x);
}

float Vector2f::length() const
{
    return std::sqrt((x * x) + (y * y));
}

Vector2f& Vector2f::normalize()
{
    Vector2f v = normalized();
    x = v.x;
    y = v.y;
    return *this;
}

Vector2f Vector2f::normalized() const
{
    const float l = length();
    return l == 0.f ?
        Vector2f(0.f, 0.f) : 
        Vector2f(x / l, y / l);
}

Vector2f& Vector2f::round()
{
    Vector2f v = rounded();
    x = v.x;
    y = v.y;
    return *this;
}

Vector2f Vector2f::rounded() const
{
    return Vector2f(std::round(x), std::round(y));
}

Vector2f Vector2f::cutToInt() const
{
    return Vector2f(int(x), int(y));
}


Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs)
{
    return Vector2f(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs)
{
    return Vector2f(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2f operator*(const Vector2f& lhs, const Vector2f& rhs)
{
    return Vector2f(lhs.x * rhs.x, lhs.y * rhs.y);
}

Vector2f operator/(const Vector2f& lhs, const Vector2f& rhs)
{
    return Vector2f(lhs.x / rhs.x, lhs.y / rhs.y);
}

Vector2f operator-(const Vector2f& v)
{
    return Vector2f(-v.x, -v.y);
}

Vector2f& Vector2f::operator+=(const Vector2f& other)
{
    *this = (*this + other);
    return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& other)
{
    *this = (*this - other);
    return *this;
}

Vector2f& Vector2f::operator*=(const Vector2f& other)
{
    *this = (*this * other);
    return *this;
}

Vector2f& Vector2f::operator/=(const Vector2f& other)
{
    *this = (*this / other);
    return *this;
}

bool operator==(const Vector2f& lhs, const Vector2f& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Vector2f& lhs, const Vector2f& rhs)
{
    return !(lhs == rhs);
}


} // namespace oge