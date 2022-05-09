#ifndef __SCENE_H__
#define __SCENE_H__

#include "components/Camera.h"

#include "aecs/Registry.h"
#include "SystemManager.h"
#include "AABBTree.h"
#include "SceneGraph.h"

#include <memory>
#include <string>

namespace oge
{


struct Removed {};

// Forward declaration of an oge::Entity
class Entity;

class SceneManager;

/**
 * @brief A scene in which a part of the game is played,
 * for example the main menu and the game over screen
 * would be two different scenes containing different
 * objects and doing different things
*/
class Scene : public aecs::Registry
{
public:
    OGE_SPECIFY_CLASS_NAME(nullptr)
    friend class SceneManager;

    Scene();
    virtual void onEnter() {}
    virtual void onExit() {}

    /** @brief Update functions that gets called every game step */
    virtual void onUpdate(const float dt) {}

    /** @brief Update function that gets called every rendering frame */
    virtual void onNonFixedUpdate(const float dt) {}

    /** @brief Function that gets called during rendering */
    virtual void onDraw(const float interp) {}

    void updateSystems(const float dt);
    void updateNonFixedSystems(const float dt);

    /** 
     * @brief returns an active oge::Entity holding a
     * camera component. Returns an invalid entity if none
     * are found
     * 
     * @return oge::Entity 
    */
    oge::Entity getCameraEntity();
    Vector2f getCameraPosition(const float interp = 1.0);

    Vector2f worldToScreen(Vector2f pos, float interp = 1.0);
    Vector2f screenToWorld(Vector2f pos, float interp = 1.0);

    // Implementation is inside SceneManager.h
    template<typename T>
    void changeTo();

    void removeEntity(aecs::Entity ent);
    void cleanupRemoved();

    oge::Entity createEntity(std::string name = "NoName");

    aecs::Registry& getWorld();

    SceneGraph* getSceneGraph();

protected:
    std::unique_ptr<SceneGraph> sceneGraph_;
    SystemManager systems_;

    // Needed to change from one state to another
    SceneManager* manager_;

private:
    using aecs::Registry::remove;
    using aecs::Registry::create;
    using aecs::Registry::get_pool;
};


} // namespace oge

#include "OgeEntity.h"

#endif // __SCENE_H__