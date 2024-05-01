#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <stdint.h>

namespace oge
{


/**
 * @brief A timer with pausing capabilities
*/
class Timer
{
public:
    using seconds = std::chrono::duration<double, std::ratio<1LL, 1LL>>;
    using ms = std::chrono::milliseconds;
    using us = std::chrono::microseconds;
    using ns = std::chrono::nanoseconds;

    using Clock = std::chrono::steady_clock;

public:
    Timer();

    void start();
    void pause();
    void unpause();

    bool isPaused();

    /** @brief Gets the measured time in milliseconds */
    uint64_t getMs();

    /** @brief Gets the measured time in microseconds */
    uint64_t getUs();

    /** @brief Gets the measured time in given `Duration` */
    template<typename Duration>
    Duration get()
    {
        if(paused_)
            return std::chrono::duration_cast<Duration>(beforeStop_);

        auto elapsed = Clock::now() - startTime_;
        return std::chrono::duration_cast<Duration>(elapsed + beforeStop_);
    }

    /** @brief Sleeps for `us` microseconds */
    static void sleep(uint64_t us);

private:
    bool paused_;

    Clock::time_point startTime_;

    // Contains the duration when the timer was stopped 
    Clock::duration beforeStop_;
};


} // namespace oge
#endif // __TIMER_H__