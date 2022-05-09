#include <iostream>
#include "sound/Audio.h" 
#include "Graphics.h"
#include "Scene.h"
#include "OgeEntity.h"
#include "OgeTest.h"
#include "SceneManager.h"
#include "OgeGame.h"
#include "InputHandler.h"
#include "AABBTree.h"

#include "components/Sprite.h"
#include "components/Script.h"
#include "components/Transform.h"

#include "OgeRandom.h"
#include "math/OgeMath.h"
#include "math/Ray.h"

using namespace oge;

/** TODO:
 * - Check SceneGraph elements removal and cleanup
 * - onCollisionExit, onCollisionStay
 * - Something something GUI
 * - SceneManager changes
 * - Stack based scenes
 */

std::ostream& operator<<(std::ostream& s, Rect r)
{
    s << "(" << r.x << ',' << r.y << ',' << r.w << ',' << r.h << ')'; 
    return s;
}

int main(int argc, char *argv[])
{
    /*auto centerRect = [](Rect r)
    {
        r.x -= r.w / 2.0;
        r.y -= r.h / 2.0;
        return r;
    };

    Graphics::create("Moje okno", 800, 600);

    Graphics& graphics = Graphics::instance();
    InputHandler& input = InputHandler::instance();

    Rect prostokat(300, 150, 300, 150);

    Rect origin(0.f, 0.f, 100.f, 100.f);

    while(!input.quit())
    {
        input.update();

        if(input.keyDown(SDLK_w)) origin.y -= 10;
        if(input.keyDown(SDLK_s)) origin.y += 10;
        if(input.keyDown(SDLK_a)) origin.x -= 10;
        if(input.keyDown(SDLK_d)) origin.x += 10;

        Ray ray = Ray::fromPoints(Vector2f(origin.x, origin.y), input.getMousePos());

        graphics.renderClear();
        graphics.drawRect(prostokat, Color::blue);
        RaycastResult result = ray.sweepRect(origin, prostokat);
        if(result)
        {
            graphics.drawLine(ray, Color::orange);
            graphics.drawRect(Rect(result.point.x - 5, result.point.y - 5, 10, 10), Color::pink);

            Rect r = origin;
            r.x += ray.dir.x;
            r.y += ray.dir.y;
            r.x += (1.0 - result.time) * std::abs(ray.dir.x) * result.normal.x;
            r.y += (1.0 - result.time) * std::abs(ray.dir.y) * result.normal.y;

            graphics.drawRect(r, Color::blue);
            graphics.drawLine(result.point, result.point + result.normal * 50, Color::cyan);
        }
        else
        {
            graphics.drawLine(ray, Color::yellow);
            Rect r = origin;
            r.x += ray.dir.x;
            r.y += ray.dir.y;
            graphics.drawRect(r, Color::blue);
        }
        graphics.renderPresent();
    }*/

    /*Graphics::create("Moje okno", 800, 600);

    auto poszerz = [](Rect r, int f)
    {
        return Rect(r.x - f, r.y - f, r.w + f*2, r.h + f*2);
    };
    
    Graphics& graphics = Graphics::instance();
    InputHandler& input = InputHandler::instance();

    Rect movable(0, 0, 100, 100);

    uint32_t highlight = 0;

    AABBTree tree(nullptr);

    while(!input.quit())
    {
        input.update();

        if(input.keyDown(SDLK_w)) movable.y -= 10;
        if(input.keyDown(SDLK_s)) movable.y += 10;
        if(input.keyDown(SDLK_a)) movable.x -= 10;
        if(input.keyDown(SDLK_d)) movable.x += 10;

        if(input.keyPressed(SDLK_p))
        {
            tree.print();
            std::cout << "\n\n";
        }

        if(input.keyPressed(SDLK_h))
        {
            std::cin >> highlight;
        }

        if(input.keyPressed(SDLK_e)) tree.insert(Entity(), movable);

        graphics.renderClear();

        auto& nodes = tree.getNodes();
        tree.traverse([&](auto& elem)
        {
            if(elem.isLeaf()) graphics.drawRect(elem.box, 0, 255, 0);
            else graphics.drawRect(poszerz(elem.box, 10), 0, 0, 255);
        });

        graphics.drawRect(movable, 0, 0, 255);
        graphics.renderPresent();
    }*/

    /*auto center = [](Rect r)
    {
        r.x -= r.w / 2.0;
        r.y -= r.h / 2.0;
        return r;
    };

    Graphics::create("Moje okno", 800, 600);

    Graphics& graphics = Graphics::instance();
    InputHandler& input = InputHandler::instance();

    Rect r1(0, 0, 100, 100);
    Rect r2(200, 200, 100, 100);

    while(!input.quit())
    {
        input.update();

        if(input.keyDown(SDLK_w)) r1.y -= 10;
        if(input.keyDown(SDLK_s)) r1.y += 10;
        if(input.keyDown(SDLK_a)) r1.x -= 10;
        if(input.keyDown(SDLK_d)) r1.x += 10;

        Vector2f r1min = r1.getMin();
        Vector2f r1max = r1.getMax();
        Vector2f r2min = r2.getMin();
        Vector2f r2max = r2.getMax();

        Vector2f min, max;
        min = Math::max(r1min, r2min);
        max = Math::min(r1max, r2max);
        if((r1min.x < r2min.x && r1min.y < r2min.x) ||
           (r1max.x > r2max.x && r1max.y > r2max.x))
        {
            std::swap(min.y, max.y);
        }

        graphics.renderClear();
        graphics.drawRect(r1, Color::blue);
        graphics.drawRect(center(Rect(min.x, min.y, 10, 10)), Color::pink);
        graphics.drawRect(center(Rect(max.x, max.y, 10, 10)), Color::yellow);
        graphics.drawRect(r2, Color::red);
        graphics.renderPresent();
    }*/

    Timer myTimer;
    Graphics::create("Moje okno", 800, 600);
    TestGame gra;
    gra.gameLoop();
    std::cout << myTimer.get<Timer::seconds>().count() << std::endl;
    return 0;
}