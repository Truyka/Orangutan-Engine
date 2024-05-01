#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "math/OgeMath.h"

#include "Scene.h"

namespace oge
{


class Renderer
{
public:
    OGE_SPECIFY_CLASS_NAME("Renderer")

    void render(Scene& scene, const double interpolate);
    void recordPositions(Scene& scene);

    bool debug_ = false;
};


} // namespace oge
#endif // __RENDERER_H__