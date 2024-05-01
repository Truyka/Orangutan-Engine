#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Config.h"

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

namespace oge
{


class Audio
{
public:
    OGE_DELETE_ASSIGN(Audio)

public:
    ~Audio();

    static Audio& instance();

    /* MUSIC */

    void playMusic(const std::string& path, float volume = 1.f, int loops = -1);
    void playMusicFadeIn(const std::string& path, int ms, float volume = 1.f, int loops = -1);
    void playMusicFadeInPos(const std::string& path, int ms, double pos, float volume = 1.f, int loops = -1);

    bool isPlayingMusic();
    bool isPausedMusic();
    void setMusicVolume(float volume);

    void pauseMusic();
    void resumeMusic();

    void deallocateMusic();

    /* SOUNDS */
    
    int playSound(const std::string& path, float volume = 1.f, int loops = 0, int channel = -1);

    bool isPlaying(const std::string& path, int channel);
    bool isPaused(const std::string& path, int channel);
    void setVolume(const std::string& path, int channel, float volume);

    void pauseSound(const std::string& path, int channel);
    void resumeSound(const std::string& path, int channel);
    void stopSound(const std::string& path, int channel);

    Mix_Chunk* getSound(const std::string& path);
    bool removeSound(const std::string& path);

    size_t clearSoundBank();

private:
    Audio();

    Mix_Music* music_;
    std::unordered_map<std::string, Mix_Chunk*> soundMap_;
};

extern Audio& audio;


} // namespace oge
#endif // __AUDIO_H__