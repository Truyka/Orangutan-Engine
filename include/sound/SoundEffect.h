#ifndef __SOUNDEFFECT_H__
#define __SOUNDEFFECT_H__

#include "OgeEntity.h"
#include "math/Vector2f.h"
#include "aecs/Registry.h"

#include <string>
#include <float.h>

namespace oge
{


class SoundEffect
{
public:
    SoundEffect(std::string _path, float _volume = 1.0, float _distance = FLT_MAX);

    SoundEffect& bind(oge::Entity ent);
    SoundEffect& bind(aecs::Entity ent, aecs::Registry& reg);

    /** 
     * @brief Plays the specified sound globally if the bound entity
     * is null, otherwise it plays it using play(oge::Entity&) with the
     * binded entity
    */
    void play();

    /**
     * @brief Play the specified sound as if was heard from
     * a given `distance`
     * 
     * @param distance distance the sound is heard from
    */
    void play(float distance);

    /**
     * @brief Plays the specified sound only if distance from
     * target and origin is less than `hearDistance`. Fades out
     * the sound when it's further than `fallOff * hearDistance` 
     * 
     * @param origin coordinates the sound originates from
     * @param target coordinates the sound is heard from
    */
    void play(const Vector2f& origin, const Vector2f& target);

    /**
     * @brief Calls play(Vector2f, Vector2f) using the center of
     * `ent` as sound origin and camera's center as the target (listener)
     * 
     * @param ent the entity from which the sound originates
    */
    void play(oge::Entity ent);
    void play(aecs::Entity ent, aecs::Registry& reg);

    bool isPlaying();
    void stop();

    void pause();
    void resume();

    int getChannel() { return channel_; }

    std::string path;
    float volume;
    float hearDistance;
    float fallOff;

private:
    int channel_;
    aecs::Entity entity_;
    aecs::Registry* reg_;
};


} // namespace oge
#endif // __SOUNDEFFECT_H__