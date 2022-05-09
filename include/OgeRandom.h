#ifndef __RNG_H__
#define __RNG_H__

#include "math/Vector2f.h"

#include <random>
#include <stdint.h>

namespace oge
{


/**
 * @brief A simple class wrapping upon std's random library functionality.
 * To use it use the global object 'RNG' or create a RandomEngine instance
*/
class RandomEngine
{
public:
    RandomEngine();

    /** @brief Returns a random integer between 'a' and 'b' (inclusive) */
    int rollInt(const int a, const int b);

    /** @brief Returns a random float between 'a' and 'b' (inclusive) */
    float rollFloat(const float a, const float b);

    /** @brief Has a 'p' percent chance of returning true */
    bool rollPercentage(const float p);

    /** @brief Returns a normalized vector pointing in a random direction */
    Vector2f rollDirection();

    void setSeed(uint32_t seed);
private:
    std::mt19937 engine_;
};

/** @brief A global object used for random number generation */
extern RandomEngine RNG;


} // namespace oge
#endif // __RNG_H__