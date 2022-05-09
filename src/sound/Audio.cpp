#include "sound/Audio.h"

#include "Config.h"

#include <SDL.h>

namespace oge
{


Audio& audio = Audio::instance();

Audio::Audio() : music_(nullptr)
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
    {
        OGE_LOG_ERROR( Mix_GetError() );
    }
}

Audio::~Audio()
{
    deallocateMusic();
    clearSoundBank();

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_Quit();
}

Audio& Audio::instance()
{
    static Audio ins;
    return ins;
}

void Audio::deallocateMusic()
{
    if(music_ != nullptr)
    {
        Mix_FreeMusic(music_);
        music_ = nullptr;
    }
}

int Audio::playSound(const std::string& path, float volume, int loops, int channel)
{
    Mix_Chunk* sound = getSound(path);
    if(sound == nullptr)
    {
        sound = Mix_LoadWAV(path.c_str());
        soundMap_[path] = sound;
    }

    int played = Mix_PlayChannel(channel, sound, loops);
    if(played < 0)
    {
        played = Mix_PlayChannel(Mix_GroupOldest(-1), sound, loops);
    }
    Mix_Volume(played, volume * MIX_MAX_VOLUME);
    return played;
}

void Audio::playMusic(const std::string& path, float volume, int loops)
{
    deallocateMusic();
    music_ = Mix_LoadMUS(path.c_str());

    Mix_PlayMusic(music_, loops);
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

void Audio::playMusicFadeIn(const std::string& path, int ms, float volume, int loops)
{
    deallocateMusic();
    music_ = Mix_LoadMUS(path.c_str());

    Mix_FadeInMusic(music_, loops, ms);
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

void Audio::playMusicFadeInPos(const std::string& path, int ms, double pos, float volume, int loops)
{
    deallocateMusic();
    music_ = Mix_LoadMUS(path.c_str());

    Mix_FadeInMusicPos(music_, loops, ms, pos);
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

void Audio::setMusicVolume(float volume)
{
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

bool Audio::isPlayingMusic()
{
    return Mix_PlayingMusic();
}

bool Audio::isPausedMusic()
{
    return Mix_PausedMusic();
}

void Audio::pauseMusic()
{
    Mix_PauseMusic();
}

void Audio::resumeMusic()
{
    Mix_ResumeMusic();
}

bool Audio::isPlaying(const std::string& path, int channel)
{
    Mix_Chunk* sound = getSound(path);

    return Mix_GetChunk(channel) == sound &&
           Mix_Playing(channel) > 0;
}

bool Audio::isPaused(const std::string& path, int channel)
{
    Mix_Chunk* sound = getSound(path);

    return Mix_GetChunk(channel) == sound &&
           Mix_Paused(channel) > 0;
}

void Audio::setVolume(const std::string& path, int channel, float volume)
{
    Mix_Chunk* sound = getSound(path);

    if(sound == Mix_GetChunk(channel))
        Mix_Volume(channel, volume * MIX_MAX_VOLUME);
}

void Audio::pauseSound(const std::string& path, int channel)
{
    Mix_Chunk* sound = getSound(path);

    if(sound == Mix_GetChunk(channel))
        Mix_Pause(channel);
}

void Audio::resumeSound(const std::string& path, int channel)
{
    Mix_Chunk* sound = getSound(path);

    if(sound == Mix_GetChunk(channel))
        Mix_Resume(channel);
}

void Audio::stopSound(const std::string& path, int channel)
{
    Mix_Chunk* sound = getSound(path);

    if(sound == Mix_GetChunk(channel))
        Mix_HaltChannel(channel);
}

Mix_Chunk* Audio::getSound(const std::string& path)
{
    auto it = soundMap_.find(path);
    return it == soundMap_.end() ? nullptr : it->second;
}

bool Audio::removeSound(const std::string& path)
{
    auto it = soundMap_.find(path);
    if(it != soundMap_.end() && it->second != nullptr)
    {
        Mix_FreeChunk(it->second);
        it->second = nullptr;
        soundMap_.erase(it);

        return true;
    }
    return false;
}

size_t Audio::clearSoundBank()
{
    size_t count = 0;
    for(auto& p : soundMap_)
    {
        if(p.second != nullptr)
        {
            Mix_FreeChunk(p.second);
            p.second = nullptr;
            count++;
        }
    }
    soundMap_.clear();
    return count;
}


} // namespace oge