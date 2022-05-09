#include "OgeRandom.h"

#include <chrono>

namespace oge
{

RandomEngine RNG;

RandomEngine::RandomEngine() : engine_(std::chrono::system_clock::now().time_since_epoch().count())
{

}

int RandomEngine::rollInt(const int a, const int b)
{
    std::uniform_int_distribution<int> dist(a, b);
    return dist(engine_);
}

float RandomEngine::rollFloat(const float a, const float b)
{
    std::uniform_real_distribution<float> dist(a, b);
    return dist(engine_);
}

bool RandomEngine::rollPercentage(const float p)
{
    std::uniform_real_distribution<float> dist(0.f, 100.f);
    return (dist(engine_) <= p);
}

Vector2f RandomEngine::rollDirection()
{
    return Vector2f(
        rollFloat(-1.0, 1.0), 
        rollFloat(-1.0, 1.0)
    ).normalize();
}

void RandomEngine::setSeed(uint32_t seed)
{
    engine_.seed(seed);
}


} // namespace oge