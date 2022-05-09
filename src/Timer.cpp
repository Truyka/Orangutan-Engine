#include "Timer.h"
#include <thread>

namespace oge
{


Timer::Timer()
{
    start();
}

void Timer::start()
{
    startTime_ = Clock::now();
    beforeStop_ = Clock::duration::zero();
    paused_ = false;
}

void Timer::pause()
{
    if(!paused_)
    {
        paused_ = true;
        beforeStop_ += (Clock::now() - startTime_);
    }
}

void Timer::unpause()
{
    if(paused_)
    {
        paused_ = false;
        startTime_ = Clock::now();
    }
}

bool Timer::isPaused()
{
    return paused_;
}

uint64_t Timer::getMs()
{
    return this->get<std::chrono::milliseconds>().count();
}

uint64_t Timer::getUs()
{
    return this->get<std::chrono::microseconds>().count();
}

void Timer::sleep(uint64_t us)
{
    Timer sleepTimer;
    while(sleepTimer.getUs() + 5000 < us)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }
    while(sleepTimer.getUs() < us);
}


} // namespace oge