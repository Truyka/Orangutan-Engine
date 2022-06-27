#ifndef __VECTOR2F_H__
#define __VECTOR2F_H__

#include <string>

namespace oge
{


class Vector2f
{
public:
    Vector2f(float val);
    Vector2f(float xc, float yc);
    Vector2f();

    std::string toString() const;

    static Vector2f fromDegrees(float degree);
    static Vector2f fromRadians(float rad);

    float toDegrees();
    float toRadians();

    float length() const;

    Vector2f  normalized() const;
    Vector2f& normalize();

    Vector2f  rounded() const;
    Vector2f& round();

    Vector2f cutToInt() const;

    Vector2f& operator+=(const Vector2f& other);
    Vector2f& operator-=(const Vector2f& other);
    Vector2f& operator*=(const Vector2f& other);
    Vector2f& operator/=(const Vector2f& other);

public:
    float x, y;  

    static Vector2f null;
};

Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs);
Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs);
Vector2f operator/(const Vector2f& lhs, const Vector2f& rhs);
Vector2f operator*(const Vector2f& lhs, const Vector2f& rhs);
Vector2f operator-(const Vector2f& v);

bool operator==(const Vector2f& lhs, const Vector2f& rhs);
bool operator!=(const Vector2f& lhs, const Vector2f& rhs);


} // namespace oge
#endif // __VECTOR2F_H__