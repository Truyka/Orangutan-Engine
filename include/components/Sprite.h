#ifndef __CSPRITE_H__
#define __CSPRITE_H__

#include "Graphics.h"
#include "math/OgeMath.h"
#include "OgeEntity.h"

#include <string>
#include <float.h>

namespace oge
{


struct Sprite
{
    Sprite(std::string p, int w = 0, int h = 0, Rect c = Rect::none) 
        : path(p), size(w, h), clip(c), renderSize(size)
    {

    }

    Sprite(std::string p, Vector2f s, Rect c = Rect::none) 
        : path(p), size(s), clip(c), renderSize(size)
    {

    }

    /**
     * @brief Returns the current center of entity ent
     * 
     * @return Vector2f 
    */
    static Vector2f getCenter(Entity ent)
    {
        Transform& t = ent.get<Transform>();
        Sprite& spr = ent.get<Sprite>();

        return t.pos + (spr.renderSize == Vector2f(0.f, 0.f) ? spr.size : spr.renderSize) / 2.f;
    }

    Vector2f getSize()
    {
        Vector2f ret;
        ret.x = (clip.w > 0 && size.x == 0 ? clip.w : size.x);
        ret.y = (clip.h > 0 && size.y == 0 ? clip.h : size.y);
        return ret;
    }

    Sprite& loadText(std::string str, Color color = Color::white, TextQuality q = TextQuality::Normal)
    {
        Graphics::instance().loadText(path, str, color, q);
        return *this;
    }

    Sprite& loadTextWrap(std::string str, Color color, int wrap, TextQuality q = TextQuality::Normal)
    {
        Graphics::instance().loadTextWrap(path, str, color, wrap, q);
        return *this;
    }
    
    Sprite& loadTextShaded(std::string str, Color col1, Color col2)
    {
        Graphics::instance().loadTextShaded(path, str, col1, col2);
        return *this;
    }

    Sprite& loadTextShadedWrap(std::string str, Color col1, Color col2, int wrap)
    {
        Graphics::instance().loadTextShadedWrap(path, str, col1, col2, wrap);
        return *this;
    }

    std::string path;
    Vector2f size;
    Vector2f renderSize;
    Rect clip;

    float scale = 1.0;
    float rotation = 0.0;
    Flip flip = Flip::None;
    Vector2f center = Vector2f::null;
    bool fixed = false;

    int zindex = 0;

    Vector2f off = Vector2f(0.f, 0.f);

    Color modColor = Color(255, 255, 255, 255);
};


} // namespace oge
#endif // __CSPRITE_H__