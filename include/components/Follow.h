#ifndef __FOLLOW_H__
#define __FOLLOW_H__

#include "aecs/Entity.h"
#include <stdint.h>

namespace oge
{


struct Follow
{
    aecs::Entity entity = aecs::Entity(SIZE_MAX, SIZE_MAX);
    double speed = 1.0;
};


} // namespace oge
#endif // __FOLLOW_H__