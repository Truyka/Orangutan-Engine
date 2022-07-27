#include "math/Rect.h"

#include <cmath>
#include "math/OgeMath.h"

namespace oge
{

const Rect Rect::none = Rect(0,0,0,0);

Rect::Rect(float cx, float cy, float cw, float ch) 
        : x(cx), y(cy), w(cw), h(ch)
{

}

Rect::Rect() : x(0.f), y(0.f), w(0.f), h(0.f)
{

}

SDL_Rect Rect::toSDLRect() const
{
    return SDL_Rect {
        int(x),
        int(y),
        int(w),
        int(h)
    };
}

Rect& Rect::round()
{
    *this = rounded();
    return *this;
}

Rect Rect::rounded() const
{
    return Rect(std::round(x), std::round(y), std::round(w), std::round(h));
}

Rect& Rect::widen(float f)
{
    *this = widened(f);
    return *this;
}

Rect Rect::widened(float f)
{
    return Rect(
        x - f,
        y - f,
        w + f * 2,
        h + f * 2
    );
}

bool Rect::isValid() const
{
    return w > 0 && h > 0;
}

bool Rect::intersects(const Rect& other) const
{
    return x < other.x + other.w &&
           y < other.y + other.h &&
           x + w > other.x &&
           y + h > other.y;
}

bool Rect::contains(const Rect& other) const
{
    return x <= other.x &&
           y <= other.y &&
           x + w >= other.x + other.w &&
           y + h >= other.y + other.h;
}

Vector2f Rect::getMin() const
{
    return Vector2f(x, y);
}

Vector2f Rect::getMax() const
{
    return Vector2f(x + w, y + h);
}

Vector2f Rect::getCenter() const
{
    return Vector2f(x + w / 2.0, y + h / 2.0);
}

Rect Rect::unionRect(const Rect& other) const
{
    return Rect::fromPoints(
        Math::min(getMin(), other.getMin()), 
        Math::max(getMax(), other.getMax())
    );
}

float Rect::area() const
{
    return w * h;
}

Rect Rect::fromPoints(const Vector2f& a, const Vector2f& b)
{
    Vector2f up = Math::min(a, b);
    Vector2f down = Math::max(a, b);

    return Rect(
        up.x,
        up.y,
        down.x - up.x,
        down.y - up.y
    );
}

Rect Rect::fromSize(const Vector2f& pos, const Vector2f& size)
{
    return Rect(pos.x, pos.y, size.x, size.y);
}


} // namespace oge