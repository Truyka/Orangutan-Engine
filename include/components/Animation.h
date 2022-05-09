#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Graphics.h"
#include "math/OgeMath.h"

#include <vector>
#include <string>

namespace oge
{


struct Animation
{
    Animation(float cinterval = 0.1, bool cloop = true, bool creset = true)
        : interval(cinterval), loop(cloop), reset(creset)
    {

    }

    struct Frame
    {
        Rect clip = Rect::none;
        Vector2f off = Vector2f(0.f, 0.f);
        float interval = 0.0;
    };

    Frame& current()
    {
        return frames.at(currentIdx);
    }

    float getInterval()
    {
        float frameInterval = current().interval;
        return frameInterval == 0 ? interval : frameInterval;
    }

    size_t framesCount()
    {
        return frames.size();
    }

    /**
     * @brief Adds multiple frames to the current Animation
     * created from given clip repeated wCount times horizontally
     * and hCount vertically with its x and y changing by its 
     * width and height 
     * 
     * @param clip defines the starting point, width and height 
     * of a single frame
     * @param wCount tells how many frames are there horizontally
     * @param hCount tells how many frames are there vertically
    */
    Animation& addFrames(Rect clip, int wCount, int hCount)
    {
        Frame frame{clip};
        for(int h = 0; h < hCount; h++)
        {
            frame.clip.x = clip.x;
            for(int w = 0; w < wCount; w++)
            {
                frames.push_back(frame);
                frame.clip.x += frame.clip.w;
            }
            frame.clip.y += frame.clip.h;
        }
        return *this;
    }

    std::vector<Frame> frames;
    size_t currentIdx = 0;
    bool loop = true;
    bool reset = true;

    float interval = 0.1;
    float elapsed  = 0.0;

    std::string path;
};

struct Animated
{
    std::vector<Animation> animations;
    size_t currentIdx;

    Animation& current()
    {
        return animations.at(currentIdx);
    }

    void changeTo(size_t n)
    {
        if(n < animations.size())
        {
            Animation& anim = current();
            if(anim.reset) anim.currentIdx = 0;
            currentIdx = n;
        }
    }

    /**
     * @brief Changes to animation at index 'n' only if it isn't
     * already played
    */
    void changeToNew(size_t n)
    {
        if(currentIdx != n)
            changeTo(n);
    }

    void add(const Animation& anim)
    {
        animations.push_back(anim);
    }
};


} // namespace oge
#endif // __ANIMATION_H__