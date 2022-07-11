#ifndef __OGEENTITY_H__
#define __OGEENTITY_H__

#include "aecs/Entity.h"
#include "Config.h"
#include "Scene.h"

#include <stdint.h>

namespace oge
{


/**
 * @brief Contains an internal entity and a pointer to
 * the stage it was created from. Provides a wrapper
 * for calling aecs::Registry methods upon a given entity
*/
class Entity
{
public:
    Entity(aecs::Entity entity, Scene* scene) 
        : entity_(entity), scene_(scene)
    {}

    Entity() : entity_(aecs::Entity::null), scene_(nullptr) 
    {}

    /* Methods wrapping aecs::Registry functionality */

    template<typename C, typename... Args>
    C& add(Args&&... args)
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        return scene_->add<C>(entity_, std::forward<Args>(args)...);
    }

    template<typename C, typename... Args>
    C& set(Args&&... args)
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        return scene_->set<C>(entity_, std::forward<Args>(args)...);
    }

    template<typename C>
    C& get()
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        return scene_->get<C>(entity_);
    }

    template<typename C>
    C* try_get()
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        return scene_->try_get<C>(entity_);
    }

    template<typename... C>
    bool has()
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        return scene_->has<C...>(entity_);
    }

    template<typename C>
    void remove()
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        scene_->removeComponent<C>(entity_);
    }

    void remove()
    {
        scene_->removeEntity(entity_);
    }

    const aecs::Entity& getEntity() const
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        return entity_;
    }

    Scene& getScene()
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a deleted entity!");
        return *scene_;
    }

    operator aecs::Entity()
    {
        return entity_;
    }

    bool isValid()
    {
        return scene_ != nullptr && entity_.isValid();
    }

private:
    aecs::Entity entity_;
    Scene* scene_;
};


} // namespace oge
#endif // __OGEENTITY_H__