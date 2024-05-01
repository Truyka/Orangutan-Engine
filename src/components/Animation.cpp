#include "Animation.h"

namespace oge
{


Animation::Animation(float cinterval, bool cloop, bool creset)
    : interval(cinterval), loop(cloop), reset(creset)
{

}

Animation::Frame& Animation::current()
{
    return frames.at(currentIdx);
}

float Animation::getInterval()
{
    float frameInterval = current().interval;
    return frameInterval == 0 ? interval : frameInterval;
}

size_t Animation::framesCount()
{
    return frames.size();
}

Animation& Animation::addFrames(Rect clip, int wCount, int hCount)
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


size_t Animated::size() const
{
    return animations.size();
}

Animation& Animated::current()
{
    return animations[currentKey];
}

void Animated::changeTo(const std::string& id)
{
    auto it = animations.find(id);
    if(it != animations.end())
    {
        Animation& anim = current();
        if(anim.reset) anim.currentIdx = 0;
        currentKey = id;
    }
}

void Animated::changeToNew(const std::string& id)
{
    if(currentKey != id)
        changeTo(id);
}

void Animated::add(const std::string& key, const Animation& anim)
{
    if(currentKey.empty())
        currentKey = key;
    animations[key] = anim;
}

Animation& Animated::operator[](const std::string &key)
{
    if(currentKey.empty())
        currentKey = key;
    return animations[key];
}


} // namespace oge