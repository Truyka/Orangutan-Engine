#ifndef __OGEGAME_H__
#define __OGEGAME_H__

#include "SceneManager.h"
#include "Renderer.h"
#include "Timer.h"

#include <chrono>

namespace oge
{


struct GameOptions
{
    using Duration = std::chrono::microseconds;

    GameOptions(int desired_frames)
    {
        basicLoop = false;
        dtSmoothening = true;

        framerate = desired_frames;
        dt = 1.f / double(framerate);

        Timer::seconds secs{dt};
        frame_time = std::chrono::duration_cast<Duration>(secs);
    }

    bool dtSmoothening;
    bool basicLoop;
    int framerate;
    double dt;
    Duration frame_time;
};

class Game
{
public:
    Game();

    void gameLoop();


    GameOptions options;

protected:
    bool quitFlag;
    SceneManager scenes_;
    Renderer renderer_;
};


} // namespcae oge
#endif // __OGEGAME_H__