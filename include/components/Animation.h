#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Graphics.h"
#include "math/OgeMath.h"

#include <vector>
#include <string>
#include <map>

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
    std::map<std::string, Animation> animations;
    std::string currentKey;

    size_t size() const
    {
        return animations.size();
    }

    Animation& current()
    {
        return animations[currentKey];
    }

    void changeTo(const std::string& id)
    {
        auto it = animations.find(id);
        if(it != animations.end())
        {
            Animation& anim = current();
            if(anim.reset) anim.currentIdx = 0;
            currentKey = id;
        }
    }

    /**
     * @brief Changes to animation at index 'n' only if it isn't
     * already played
    */
    void changeToNew(const std::string& id)
    {
        if(currentKey != id)
            changeTo(id);
    }

    void add(const std::string& key, const Animation& anim)
    {
        if(currentKey.empty())
            currentKey = key;
        animations[key] = anim;
    }

    Animation& operator[](const std::string &key)
    {
        if(currentKey.empty())
            currentKey = key;
        return animations[key];
    }
};


} // namespace oge
#endif // __ANIMATION_H__