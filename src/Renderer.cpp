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
#include <queue>

namespace oge
{


void Renderer::render(Scene& scene, const double interpolate)
{
    Graphics& graphics = Graphics::instance();

    Rect window = graphics.getWindowRect();
    window.x = 0;
    window.y = 0;

    Vector2f cameraPos = scene.getCameraPosition(interpolate).round();
    float zoom = scene.getCameraZoom();

    struct RenderInfo
    {
        TextureInfo info;
        Rect renderRect;
        Sprite spr;
        aecs::Entity ent;
    };

    // Workaround to reserve space in a priority queue
    std::vector<RenderInfo> renderTargetsTemp;
    renderTargetsTemp.reserve(1024);

    auto comp = [](const RenderInfo& a, const RenderInfo& b)
    {
        // For consistency we're rendering newer entities first
        // if zindex is the same
        if(a.spr.zindex == b.spr.zindex)
            return a.ent.index > b.ent.index;
        
        return a.spr.zindex > b.spr.zindex;
    };

    std::priority_queue<RenderInfo, std::vector<RenderInfo>, decltype(comp)> renderTargets (
        comp, std::move(renderTargetsTemp)
    );

    auto view = scene.view<Sprite, Transform>();
    for(const auto& entity : view)
    {
        Sprite& sprite = scene.get<Sprite>(entity);
        Transform& tran = scene.get<Transform>(entity);

        Vector2f coords = tran.pos;
        Vector2f size = sprite.getSize();

        TextureInfo info = graphics.loadOrGet(sprite.path);
        if(size.x == 0) size.x = info.width;
        if(size.y == 0) size.y = info.height;

        coords = Math::lerp(tran.oldPos.rounded(), tran.pos.rounded(), interpolate);

        coords += sprite.off;
        coords -= !sprite.fixed * cameraPos;

        size *= sprite.scale * tran.scale;

        Rect renderRect(
            coords.x * zoom, 
            coords.y * zoom, 
            size.x * zoom, 
            size.y * zoom
        );

        if(window.intersects(renderRect))
        {
            renderTargets.push(RenderInfo{info, renderRect, sprite, entity});
        }

        sprite.renderSize = size;
    }

    while(!renderTargets.empty())
    {
        RenderInfo i = renderTargets.top();
        renderTargets.pop();

        graphics.render(i.info, i.renderRect, i.spr.clip, i.spr.rotation, i.spr.center, i.spr.flip, i.spr.modColor);
    }

    if(debug_)
    {
        // Debug staff - draws the hitboxes 
        scene.getSceneGraph()->draw(cameraPos);

        /*auto view2 = scene.view<Collider, Transform>();
        for(auto& entity : view2)
        {
            Rect draw = getHitbox(scene, entity);
            draw.x -= cameraPos.x;
            draw.y -= cameraPos.y;

            graphics.drawRect(draw, Color::red);
        }*/
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