#include "sound/SoundEffect.h"

#include "sound/Audio.h"

#include "components/Camera.h"
#include "components/Transform.h"
#include "components/Sprite.h"

namespace oge
{


SoundEffect::SoundEffect(std::string _path, float _volume, float _distance)
    : path(_path), volume(_volume), hearDistance(_distance), channel_(0), fallOff(0.3), reg_(nullptr)
{

}

SoundEffect& SoundEffect::bind(oge::Entity ent)
{
    this->bind(ent.getEntity(), ent.getScene());
    return *this;
}

SoundEffect& SoundEffect::bind(aecs::Entity ent, aecs::Registry& reg)
{
    entity_ = ent;
    reg_ = &reg;
    return *this;
}

void SoundEffect::play()
{
    if(entity_.isValid())
    {
        this->play(entity_, *reg_);
    }
    else
    {
        channel_ = audio.playSound(path, volume);
    }
}

void SoundEffect::play(float distance)
{
    if(distance > hearDistance)
        return;

    float smoothen = 1.f;
    if(hearDistance * fallOff > 0)
        smoothen = std::min(1.f, (hearDistance - distance) / (hearDistance * fallOff));

    channel_ = audio.playSound(path, smoothen * volume);
}

void SoundEffect::play(const Vector2f& origin, const Vector2f& target)
{
    float distance = (target - origin).length();
    this->play(distance);
}

void SoundEffect::play(oge::Entity ent)
{   
    this->play(ent.getEntity(), ent.getScene());
}

void SoundEffect::play(aecs::Entity ent, aecs::Registry& reg)
{
    Vector2f pos = reg.get<Transform>(ent).pos;
    Sprite* spr = reg.try_get<Sprite>(ent);
    if(spr != nullptr)
    {
        pos += spr->renderSize / 2.f;
    }

    Vector2f camera = Camera::getCurrentCenter(reg);
    this->play(pos, camera);
}

bool SoundEffect::isPlaying()
{
    return audio.isPlaying(path, channel_);
}

void SoundEffect::stop()
{
    audio.stopSound(path, channel_);
}

void SoundEffect::pause()
{
    audio.pauseSound(path, channel_);
}

void SoundEffect::resume()
{
    audio.resumeSound(path, channel_);
}


} // namespace oge