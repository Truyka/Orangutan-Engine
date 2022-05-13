#include "OgeTest.h"
#include "OgeEntity.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "InputHandler.h"
#include "Config.h"

#include "SystemManager.h"
#include "OgeRandom.h"
#include "Utility.h"
#include "sound/Audio.h"
#include "sound/SoundEffect.h"

#include "systems/ScriptSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/FollowSystem.h"
#include "systems/PhysicsSystem.h"

#include "components/Meta.h"
#include "components/Transform.h"
#include "components/Sprite.h"
#include "components/Script.h"
#include "components/Camera.h"
#include "components/Follow.h"
#include "components/Physics.h"

#include <float.h>
#include <math.h>

using namespace oge;


struct Foo
{
    int counter = 0;
};

class SideToSide : public oge::Scriptable
{
public:
    OGE_SPECIFY_CLASS_NAME("SideToSide");

    void onDetach()
    {
        std::cout << "SideToSide detached\n";
    }

    void onUpdate(const float dt) override
    {
        static int counter = 0;
        Transform& t = entity_.get<Transform>();

        t.pos.x += 10 * sin(float(counter++) / 10);
    }
};

std::ostream& operator<<(std::ostream& s, Vector2f v)
{
    s << "(" << v.x << ',' << v.y << ')'; 
    return s;
}

class CoolFollow : public Scriptable
{
public:
    CoolFollow(Entity e) : ent(e) {}

    void onCollisionEnter(CollisionInfo info) override
    {
        if(info.object.get<Meta>().name == "Gracz")
        {
            entity_.remove();
        }
    }

    void onUpdate(const float dt) override
    {
        Transform& t1 = entity_.get<Transform>();
        Transform& t2 = ent.get<Transform>();

        Vector2f distance = t2.pos - t1.pos;
        
        entity_.get<Velocity>().vel = distance.normalize() * 200 * dt;
    }

    Entity ent;
};

class MoveScript : public oge::Scriptable
{
public:
    MoveScript(SDL_Keycode w, SDL_Keycode s, SDL_Keycode a, SDL_Keycode d)
        : wKey(w), sKey(s), aKey(a), dKey(d)
    {

    }

    void onDetach()
    {
        std::cout << "moveScript detached\n";
    }

    void onCollisionEnter(CollisionInfo info) override
    {
        //std::cout << entity_.getEntity().index << " collided with " << info.object.getEntity().index << std::endl;
    }

    void onUpdate(const float dt) override
    {
        SceneGraph* graph = scene_->getSceneGraph();

        Transform& t = entity_.get<Transform>();
        Sprite& spr = entity_.get<Sprite>();

        Velocity& vel = entity_.get<Velocity>();
        displacement = Vector2f(0.f, 0.f);

        if(input.keyDown(wKey)) displacement.y -= 300 * dt;
        if(input.keyDown(sKey)) displacement.y += 300 * dt;
        if(input.keyDown(aKey)) displacement.x -= 300 * dt;
        if(input.keyDown(dKey)) displacement.x += 300 * dt;
        if(input.keyDown(SDLK_u)) t.scale += 0.01;
        if(input.keyDown(SDLK_j)) t.scale -= 0.01;
        vel.vel = displacement;
        if(input.keyDown(SDLK_p))
        {
            Entity ent = scene_->getCameraEntity();
            if(ent.isValid())
            {
                Camera& c = ent.get<Camera>();
                c.shake(3, 1);
            }
        }

        Graphics::instance().loadText("tekst1", t.pos.toString(), Color::white, TextQuality::High);

        if(input.keyDown(SDLK_z)) detach();
        if(input.keyDown(SDLK_r))
        {
            Entity entity = scene_->createEntity();
            entity.add<Transform>(t.pos + 500 * RNG.rollDirection());
            entity.add<Velocity>(Vector2f(0.f, 0.f));
            entity.add<Sprite>("orangutan.png", 100, 100);
            entity.add<Collider>(Rect(0, 0, 100, 100));
            entity.add<ScriptList>()
                .bind<CoolFollow>(entity_)
                .bind<CollisionResolver>([](auto...){ return true; });
        }
    }

    Vector2f displacement = Vector2f(0.f, 0.f);

    SDL_Keycode wKey;
    SDL_Keycode sKey;
    SDL_Keycode aKey;
    SDL_Keycode dKey;
};

class SwitchScript : public oge::Scriptable
{
public:
    SwitchScript() : has(true) {}

    void onDetach()
    {
        //std::cout << "switchScript detached\n";
    }

    void onUpdate(const float dt) override
    {
        if(InputHandler::instance().keyPressed(SDLK_p))
        {
            if(has)
            {
                scriptList_->get<MoveScript>()->detach();
                scriptList_->bind<SideToSide>();
                has = false;
            }
            else
            {
                scriptList_->get<SideToSide>()->detach();
                scriptList_->bind<MoveScript>(SDLK_UP, SDLK_DOWN, SDLK_RIGHT, SDLK_LEFT);
                has = true;
            }
        }
    }

    bool has;
};

class SoundScript : public Scriptable
{
public:
    void onUpdate(const float dt) override
    {
        SoundEffect& sound = entity_.get<SoundEffect>();
        if(input.keyPressed(SDLK_h))
            sound.play(entity_);
    }
};

/** TODO:
 * Dodawanie ScriptList w trakcie onUpdate skryptu
 * Poczytaj nt. zastapienia dynamic_casta static_castem w aecs::registry
 * Sprawdz cleanup i ogarnij oge::Graphics
 * Szybko poruszajca sie obiekty blokuja sie o orangutanow ustawionych w rzadku przy dampingu i akceleracji
 * Moze cos innego zamiast zaokraglania, zaokraglanie skacze jezeli pozycja ma 0.5 po przecinku
 * Konstruktor do Follow
*/

MainScene::MainScene()
{
    systems_.add<ScriptSystem>();
    systems_.add<PhysicsSystem>();
    systems_.add<FollowSystem>();

    Entity entity = createEntity("Gracz");
    entity.add<Transform>(0, 0);
    entity.add<Velocity>(Vector2f(0, 0));
    entity.add<Sprite>("orangutan.png", 100, 100).zindex = 1;
    entity.add<ScriptList>().bind<MoveScript>(SDLK_w, SDLK_s, SDLK_a, SDLK_d);
    entity.add<ScriptList>().bind<CollisionResolver>([](auto... args){ return true; });
    entity.add<Collider>(Rect(0, 0, 100, 100));

    Entity entity2 = createEntity();
    entity2.add<Transform>(0, 0);
    entity2.add<Velocity>(Vector2f(0, 0));
    entity2.add<Sprite>("orangutan.png", 100, 100);
    entity2.add<Collider>(Rect(0, 0, 100, 100));
    entity2.add<SoundEffect>("coins.wav", 0.4f, 500.f);
    entity2.add<ScriptList>().bind<SoundScript>();
 
    Graphics::instance().setFont("arial.ttf", 24);

    Entity entity3 = createEntity();
    entity3.add<Transform>(-100, -200);
    entity3.add<Sprite>("tekst1").loadText("Jakis tekst");

    Entity camera = createEntity();
    camera.add<Transform>(0, 0);
    camera.add<Camera>(CameraType::Centered, Vector2f(100/2, 100/2));
    camera.add<Follow>(entity, 0.3f);

    for(int i = 0; i < 10; i++)
    {
        Entity ent = createEntity();
        ent.add<Transform>(-200, 100*i);
        ent.add<Sprite>("orangutan.png", 100, 100);
        ent.add<Collider>(Rect(0, 0, 100, 100));
    }

    for(int i = 0; i < 100; i++)
    {
        Entity ent = createEntity();
        ent.add<Transform>(100*i, 200);
        ent.add<Sprite>("orangutan.png", 100, 100);
        ent.add<Collider>(Rect(0, 0, 100, 100));
    }

    /*Entity who = entity;
    for(int i = 0; i < 500; i++)
    {
        Entity orangutan = createEntity();
        orangutan.add<Transform>(rand() % 5000 + 100, rand() % 5000);
        orangutan.add<Sprite>("orangutan.png", 100, 100);
        orangutan.add<Collider>(Rect(0, 0, 100, 100));
        //orangutan.add<Follow>(who, 0.3);
        who = orangutan;
    }*/
}

void MainScene::onDraw(const float interp)
{

}

TestGame::TestGame()
{
    scenes_.add<MainScene>();
}