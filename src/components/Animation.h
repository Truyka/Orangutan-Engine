#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "math/OgeMath.h"

#include <vector>
#include <string>
#include <map>

namespace oge
{


struct Animation
{
    Animation(float cinterval = 0.1, bool cloop = true, bool creset = true);

    struct Frame
    {
        Rect clip = Rect::none;
        Vector2f off = Vector2f(0.f, 0.f);
        float interval = 0.0;
    };

    Frame& current();

    float getInterval();

    size_t framesCount();

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
    Animation& addFrames(Rect clip, int wCount, int hCount);

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

    size_t size() const;

    Animation& current();

    /**
     * @brief Changes to animation at index 'n' only if it isn't
     * already played
    */
    void changeToNew(const std::string& id);
    void changeTo(const std::string& id);

    
    void add(const std::string& key, const Animation& anim);

    Animation& operator[](const std::string &key);
};


} // namespace oge
#endif // __ANIMATION_H__