#ifndef __SRC_COLLISION_SCENEGRAPH__
#define __SRC_COLLISION_SCENEGRAPH__

#include "aecs/Registry.h"
#include "math/Rect.h"
#include "math/Ray.h"
#include "math/Vector2f.h"

#include <vector>

namespace oge
{



class Scene;

class SceneGraph
{
public:
    using RaycastPair = std::pair<aecs::Entity, RaycastResult>;

public:
    SceneGraph(Scene* scene) : scene_(scene)
    {

    }

    virtual void insert(aecs::Entity ent, const Rect& box) = 0;
    virtual std::vector<aecs::Entity> query(const Rect& box) = 0;
    virtual std::vector<RaycastPair> raycast(const Ray& ray) { return {}; }
    virtual std::vector<RaycastPair> raycastBox(const Ray& ray, const Rect& box) { return {}; }

    virtual void reserve(size_t n) {}
    virtual void update() {}
    virtual void cleanupRemoved() {}

    virtual void draw(const Vector2f& camera) {}

protected:
    Scene* scene_;
};


} // namespace oge
#endif /* __SRC_COLLISION_SCENEGRAPH__ */
