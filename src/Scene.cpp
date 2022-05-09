#include "Scene.h"
#include "Graphics.h"
#include "OgeEntity.h"
#include "Components/Meta.h"
#include "OgeRandom.h"
#include "math/OgeMath.h"

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

Vector2f Scene::worldToScreen(Vector2f pos, float interp)
{
    return pos - getCameraPosition(interp);
}

Vector2f Scene::screenToWorld(Vector2f pos, float interp)
{
    return pos + getCameraPosition(interp);
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