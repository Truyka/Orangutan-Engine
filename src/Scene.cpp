#include "Scene.h"
#include "Graphics.h"
#include "OgeEntity.h"
#include "Components/Meta.h"
#include "Components/Camera.h"
#include "OgeRandom.h"
#include "math/OgeMath.h"

#include "collision/AABBTree.h"

#include <cstdlib>
#include <math.h>

namespace oge
{


Scene::Scene() : systems_(this), manager_(nullptr), sceneGraph_(new AABBTree(this))
{
    
}

oge::Entity Scene::createEntity(std::string name)
{
    oge::Entity ent(create(), this);
    ent.add<Meta>(name);
    return ent;
}

aecs::Registry& Scene::getWorld()
{
    return static_cast<aecs::Registry&>(*this);
}

SceneGraph* Scene::getSceneGraph()
{
    return sceneGraph_.get();
}

void Scene::updateSystems(const float dt)
{
    systems_.update(dt);
}

void Scene::updateNonFixedSystems(const float dt) 
{
    systems_.nonFixedUpdate(dt);
}

oge::Entity Scene::getCameraEntity()
{
    aecs::Entity ent = Camera::getCurrent(*this);
    return ent.isValid() ?
        oge::Entity(ent, this) :
        oge::Entity();
}

Vector2f Scene::getCameraPosition(const float interp)
{
    return Camera::getCurrentPosition(*this, interp); 
}

float Scene::getCameraZoom()
{
    Entity ent = getCameraEntity();
    return ent.isValid() ?
        ent.get<Camera>().zoom :
        1.f;
}

Vector2f Scene::worldToScreen(Vector2f pos, float interp)
{
    return (pos - getCameraPosition(interp)) * getCameraZoom();
}

Vector2f Scene::screenToWorld(Vector2f pos, float interp)
{
    return pos / getCameraZoom() + getCameraPosition(interp);
}

Rect Scene::worldToScreen(Rect r, float interp)
{
    Vector2f origin(r.x, r.y);
    Vector2f size(r.w, r.h);

    origin = worldToScreen(origin, interp);
    size *= getCameraZoom();

    return Rect(origin.x, origin.y, size.x, size.y);
}

Rect Scene::screenToWorld(Rect r, float interp)
{
    Vector2f origin(r.x, r.y);
    Vector2f size(r.w, r.h);

    origin = screenToWorld(origin, interp);
    size /= getCameraZoom();

    return Rect(origin.x, origin.y, size.x, size.y);
}

void Scene::removeEntity(aecs::Entity ent)
{
    this->add<Removed>(ent);
}

void Scene::cleanupRemoved()
{
    std::vector<aecs::Entity> removed = this->view<Removed>().getInnerContainer();
    for(auto& entity : removed)
    {
        if(!entity.isValid()) 
            continue;
        this->remove(entity);
    }
    sceneGraph_->cleanupRemoved();
}


} // namespace oge