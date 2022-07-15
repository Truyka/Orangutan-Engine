#include "PhysicsSystem.h"
#include "Scene.h"

#include "components/Physics.h"
#include "components/Transform.h"
#include "components/Collider.h"
#include "components/Script.h"

namespace oge
{


void PhysicsSystem::onUpdate(const float dt)
{
    // Update the velocities
    {
        auto view = scene_->view<Velocity>();
        for(const auto& entity : view)
        {
            Velocity& vel = scene_->get<Velocity>(entity);

            Acceleration* accel = scene_->try_get<Acceleration>(entity);
            Damping* damping = scene_->try_get<Damping>(entity);

            Vector2f damp(0.f, 0.f);
            if(damping != nullptr)
            {
                damp = -vel.vel * damping->damping;
            }

            Vector2f acc(0.f, 0.f);
            if(accel != nullptr)
            {
                // The dt is needed here to convert
                // received velocity gain into pixels/second
                acc = accel->acc * dt;
            }

            vel.vel += acc + damp;
        }
    }

    SceneGraph* graph = scene_->getSceneGraph();

    auto view = scene_->view<Transform, Collider>();
    for(auto& entity : view)
    {
        Collider& col = scene_->get<Collider>(entity);
        if(!col.added)
        {
            Rect hitbox = getHitbox(*scene_, entity);

            graph->insert(entity, hitbox);
            col.added = true;
        }
    }

    graph->update();

    // We need to handle moving entities first. We'll store not-moving
    // entities we notice while looking for moving ones
    std::vector<aecs::Entity> staticEntities;
    staticEntities.reserve(view.size());

    for(auto& entity : view)
    {
        Collider& col = scene_->get<Collider>(entity);
        Transform& tran = scene_->get<Transform>(entity);
        if(col.skip) continue;

        Velocity* vel = scene_->try_get<Velocity>(entity);
        if(vel == nullptr || vel->vel == Vector2f(0.f, 0.f)) 
        {
            staticEntities.push_back(entity);
            continue;
        }

        ScriptList* sl = scene_->try_get<ScriptList>(entity);
        if(sl == nullptr) continue;

        Rect hitbox = computeHitbox(tran, col);
        Ray ray(Vector2f(hitbox.x, hitbox.y), vel->vel);

        auto collisions = graph->raycastBox(ray, hitbox);

        // Sort the collisions by the time they happen
        std::sort(collisions.begin(), collisions.end(), 
            [&](const SceneGraph::RaycastPair& p1, const SceneGraph::RaycastPair& p2)
            {
                if(p1.second.time == p2.second.time)
                {
                    const Vector2f& pos1 = scene_->get<Transform>(p1.first).pos;
                    const Vector2f& pos2 = scene_->get<Transform>(p2.first).pos;

                    float dist1 = std::pow(pos1.x - tran.pos.x, 2) + std::pow(pos1.y - tran.pos.y, 2);
                    float dist2 = std::pow(pos2.x - tran.pos.x, 2) + std::pow(pos2.y - tran.pos.y, 2);

                    return dist1 < dist2;
                }
                else
                    return p1.second.time < p2.second.time;
            }
        );

        // Save the velocity so we can check if it changed
        Velocity savedVel = *vel;

        // We need to handle earlier collisions first
        for(int i = 0; i < collisions.size(); i++)
        {
            // Skip collision with ourselves
            if(collisions[i].first == entity) continue;

            RaycastResult result = collisions[i].second;

            // Check collision again if velocity has changed due to onCollisionEnter function
            if(savedVel.vel != vel->vel)
            {
                Rect objectRect = getHitbox(*scene_, collisions[i].first);

                ray.setDirection(vel->vel);
                result = ray.sweepRect(hitbox, objectRect);

                savedVel = *vel;
            }

            if(result)
            {
                CollisionInfo info
                {
                    .object = Entity(collisions[i].first, scene_),
                    .normal = result.normal,
                    .time = result.time
                };
                
                for(auto& script : sl->scripts)
                    if(script->valid())
                        script->onCollisionEnter(info);
            }
        }
    }

    // Update the positions
    {
        auto view = scene_->view<Velocity, Transform>();
        view.each([&](Velocity& vel, Transform& t)
        {
            t.pos += vel.vel;
        });
    }

    // Update the graph because entities moved
    graph->update();

    for(auto& entity : staticEntities)
    {
        Collider& col = scene_->get<Collider>(entity);
        Transform& tran = scene_->get<Transform>(entity);
        if(col.skip) continue;

        Rect hitbox = computeHitbox(tran, col);

        ScriptList* sl = scene_->try_get<ScriptList>(entity); 
        if(sl == nullptr) continue;

        auto collisions = graph->query(hitbox);
        for(int i = 0; i < collisions.size(); i++)
        {  
            if(collisions[i] == entity) continue;

            CollisionInfo info{Entity(collisions[i], scene_)};
            for(auto& script : sl->scripts)
                if(script->valid())
                    script->onCollisionEnter(info);
        }
    }
}


} // nmaespace oge