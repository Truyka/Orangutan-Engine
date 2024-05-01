#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "OgeEntity.h"
#include "Config.h"

#include "components/Collider.h"

#include <list>
#include <memory>
#include <utility>
#include <stdint.h>
#include <limits.h>

namespace oge
{


class ScriptList;

/**
 * @brief a class used for making scripts. Derive your
 * classes from it and append them to a ScriptList component
*/
class Scriptable
{
public:
    OGE_SPECIFY_CLASS_NAME(nullptr)

    Scriptable() : scriptList_(nullptr), detached_(false), scene_(nullptr)
    {}

    /** @brief executes when instantiating the script on ScriptSystem onUpdate. */
    virtual void onAttach() {}

    virtual void onUpdate(const float) {}
    virtual void onNonFixedUpdate(const float) {}
    virtual void onCollisionEnter(CollisionInfo info) {}

    virtual void onDetach() {}

    void instatiate(Entity entity, ScriptList* scriptlist)
    {
        entity_ = entity;
        scriptList_ = scriptlist;
        scene_ = &entity.getScene();
    }

    bool valid()    
    {
        return scriptList_ != nullptr;
    }

    void detach()
    {
        detached_ = true;
    }

    bool isRemoved()
    {
        return detached_;
    }

    uint32_t getId()
    {
        return id_;
    }

protected:
    bool detached_;
    Entity entity_;
    Scene* scene_;
    ScriptList* scriptList_;

private:
    friend class ScriptList;

    uint32_t id_;
};

/**
 * @brief Used only for creating a special id
 * for every script so it may be searched using 
 * ScriptList::get<>()
*/
class ScriptIndexator
{
private:
    friend class ScriptList;

    static uint32_t next()
    {
        static uint32_t id = 0;
        return id++; 
    }

    template<typename T>
    static uint32_t index()
    {
        const static uint32_t id = next();
        return id;
    }
};

/**
 * @brief Component class for attaching scripts to entiteis
*/
struct ScriptList
{
    using script_ptr = std::unique_ptr<Scriptable>;

    std::vector<script_ptr> scripts;

    /**
     * @brief Appends a new script to the scripts list. The
     * script is created empty and should be used only after
     * its instantiation in ScriptSystem
     * 
     * @tparam T your custom class inherited from oge::Scriptable
     * @param args additional arguments passed to T's ctor
    */
    template<typename T, typename... Args>
    ScriptList& bind(Args&&... args)
    {
        script_ptr script = std::make_unique<T>(std::forward<Args>(args)...);
        script->id_ = ScriptIndexator::index<T>();
        scripts.push_back( std::move(script) );
        return *this;
    }

    /**
     * @brief Searches linearly for your script in script list
     * and returns a pointer to it
     * 
     * @return T* pointer to given script or nullptr if not found
    */
    template<typename T>
    T* get()
    {
        uint32_t id = ScriptIndexator::index<T>();
        for(auto& sc : scripts)
        {
            if(sc->getId() == id)
                return static_cast<T*>(sc.get());
        }
        return nullptr;
    }
};


} // namespace oge
#endif // __SCRIPT_H__