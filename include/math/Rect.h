#ifndef __RECT_H__
#define __RECT_H__

#include "Vector2f.h"
#include <SDL.h>

namespace oge
{


class Rect
{
public:
    Rect(float cx, float cy, float cw, float ch);
    Rect();
    static const Rect none;

    /**
     * @brief Converts this rect to SDL_Rect by implicitly
     * converting floats to ints
    */
    SDL_Rect toSDLRect() const;

    Rect& round();
    Rect  rounded() const;

    Rect& widen(float f);
    Rect  widened(float f);

    Rect cutToInt() const;

    bool isValid() const;

    static Rect fromPoints(const Vector2f& a, const Vector2f& b);
    static Rect fromSize(const Vector2f& pos, const Vector2f& size);

    bool intersects(const Rect& other) const;
    bool contains(const Rect& other) const;

    /** @brief Gets the coordinates of the top left corner */
    Vector2f getMin() const;

    /** @brief Gets the coordinates of the bottom right corner */
    Vector2f getMax() const;

    /** @brief Returns the center point of the rect */
    Vector2f getCenter() const;

    Rect unionRect(const Rect& other) const;

    float area() const;

public:
    float x, y, w, h;
};


} // namespace oge
#endif // __RECT_H__