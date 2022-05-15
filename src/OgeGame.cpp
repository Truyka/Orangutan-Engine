#include "OgeGame.h"

#include "Graphics.h"
#include "InputHandler.h"
#include "Timer.h"

#include <stdint.h>
#include <cmath>

namespace oge
{


Game::Game() : quitFlag(false), options(60)
{

}

void Game::gameLoop()
{
    using namespace std::chrono;

    Graphics& graphics  = Graphics::instance();

    using GameDuration = GameOptions::Duration;

    uint32_t bufferIdx = 0;
    const int bufferSize = 4;
    GameDuration dtBuffer[bufferSize];
    for(int i = 0; i < bufferSize; i++)
        dtBuffer[i] = options.frame_time;

    GameDuration accumulator{0};
    GameDuration lastFrame = options.frame_time;

    GameDuration accLimit = duration_cast<GameDuration>(Timer::seconds{8.0/60.0});

    Timer frameTimer;
    while(!quitFlag)
    {
        frameTimer.start();

        Scene& current = scenes_.current();

        // Average delta time from the last N frames
        if(options.dtSmoothening)
        {
            GameDuration dtBufferSum{0};
            for(int i = 0; i < bufferSize; i++)
            {
                dtBufferSum += dtBuffer[i];
            }
            lastFrame = (dtBufferSum / bufferSize);
        }

        accumulator += lastFrame;
        if(accumulator > accLimit)
            accumulator = accLimit;

        if(options.basicLoop)
            accumulator = options.frame_time;

        // Updates the game in fixed steps so it runs the same
        // on every framerate
        while(accumulator >= options.frame_time)
        {
            input.update();
            if(input.quit()) quitFlag = true;

            if(input.keyPressed(SDLK_F1))
                renderer_.debug_ = !renderer_.debug_;

            renderer_.recordPositions(current);

            current.onUpdate(options.dt);
            current.updateSystems(options.dt);

            current.cleanupRemoved();

            accumulator -= options.frame_time;
        }

        // Calculate how much time the last frame took in seconds 
        const double frameTimeSecs = duration_cast<Timer::seconds>(lastFrame).count();

        current.onNonFixedUpdate(frameTimeSecs);
        current.updateNonFixedSystems(frameTimeSecs);

        current.cleanupRemoved();

        /* =RENDERING= */

        // Calculate how far are we into the current frame
        float alpha = float(accumulator.count()) / float(options.frame_time.count());
        if(options.basicLoop)
            alpha = 1.0;
        
        graphics.renderClear();
        renderer_.render(current, alpha);
        current.onDraw(alpha);
        graphics.renderPresent();

        lastFrame = frameTimer.get<GameDuration>();
        dtBuffer[bufferIdx++ % bufferSize] = lastFrame;

        if(options.basicLoop)
        {
            if(lastFrame < options.frame_time * 0.98)
                Timer::sleep(duration_cast<Timer::us>(options.frame_time * 0.98 - lastFrame).count());
        }
    }
}


} // namespace oge