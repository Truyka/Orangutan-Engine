#include "Renderer.h"
#include "Graphics.h"
#include "Scene.h"

#include "components/Transform.h"
#include "components/Collider.h"
#include "components/Sprite.h"
#include "components/Camera.h"
#include "components/Follow.h"
#include "math/OgeMath.h"
#include "OgeRandom.h"

#include <float.h>
#include <math.h>
#include <iostream>

namespace oge
{


void Renderer::render(Scene& scene, const double interpolate)
{
    Graphics& graphics = Graphics::instance();

    Rect window = graphics.getWindowRect();
    window.x = 0;
    window.y = 0;

    Vector2f cameraPos = scene.getCameraPosition(interpolate);

    auto view = scene.view<Sprite, Transform>();
    view.each([&](Sprite& sprite, Transform& tran)
    {
        Vector2f coords = tran.pos;
        Vector2f size = sprite.getSize();

        TextureInfo info = graphics.loadOrGet(sprite.path);
        if(size.x == 0) size.x = info.width;
        if(size.y == 0) size.y = info.height;

        coords = Math::lerp(tran.oldPos, tran.pos, interpolate);

        coords += sprite.off;
        coords -= cameraPos;

        size *= sprite.scale * tran.scale;

        Rect renderRect(coords.x, coords.y, size.x, size.y);
        if(window.intersects(renderRect))
        {
            graphics.render(info, renderRect, sprite.clip, sprite.rotation, sprite.center, sprite.flip);
        }

        sprite.renderSize = size;
    });

    if(debug_)
    {
        // Debug staff - draws the hitboxes 
        scene.getSceneGraph()->draw(cameraPos);

        auto view2 = scene.view<Collider, Transform>();
        for(auto& entity : view2)
        {
            Rect draw = getHitbox(scene, entity);
            draw.x -= cameraPos.x;
            draw.y -= cameraPos.y;

            graphics.drawRect(draw, Color::red);
        }
    }
}

void Renderer::recordPositions(Scene& scene)
{
    {
        auto view = scene.view<Transform>();
        view.each([&](Transform& t)
        {
            t.oldPos = t.pos;
        });
    }
}


} // namespace oge