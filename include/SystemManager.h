#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMMANAGER_H__

#include "Config.h"

#include <unordered_map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <string>

namespace oge
{


class Scene;
class SystemManager;
class System
{
public:
    OGE_SPECIFY_CLASS_NAME(nullptr)

    System() : scene_(nullptr) {}

    virtual void onCreate() {}
    virtual void onEnter() {}
    virtual void onUpdate(const float) {}
    virtual void onNonFixedUpdate(const float) {}
    virtual void onExit() {}
    virtual void onDestroy() {}

    Scene& getScene()
    {
        OGE_ASSERT(scene_ != nullptr, "Trying to use a null initialized scene!");
        return *scene_;
    }

    friend class SystemManager;
protected:
    Scene* scene_;
};



/**
 * @brief Provides an interface for adding systems to a vector
 * and updating them linearly. You can also retrive different
 * systems by their class.
 */

class SystemManager
{
public:
    using system_ptr = std::unique_ptr<System>;

public:
    SystemManager(Scene* Scene) : scene_(Scene)
    {

    }

    /**
     * @brief Constructs a new system and appends it to the 
     * system list. Sets the 'system' member pointer 'scene_'
     * to the Scene it was initialized in
     * 
     * @param args arguments to the system ctor 
    */
    template<typename T, typename... Args>
    void add(Args&&... args)
    {
        static_assert(std::is_base_of_v<System, T>, "The type you're adding has to be derived from oge::System");

        OGE_LOG_INFO("Adding a new system to the SystemList: " << OGE_GET_CLASS_NAME(T));

        std::type_index typeidx(typeid(T));
        if(typeToIndex_.find(typeidx) != typeToIndex_.end())
        {
            OGE_LOG_WARNING("SystemManager already has a system of name: " << OGE_GET_CLASS_NAME(T));
            return;
        }

        system_ptr ptr = std::make_unique<T>(std::forward<Args>(args)...);
        ptr->scene_ = scene_;

        typeToIndex_[typeidx] = systems_.size();
        systems_.push_back(std::move(ptr));
    }


    /**
     * @brief Returns a reference to the given system of type T
     * 
     * @warning may throw an exception if the given system wasn't
     * added
     * 
     * @return T& a reference to the right system
    */
    template<typename T>
    T& get()
    {
        static_assert(std::is_base_of_v<System, T>, "The type you're getting has to be derived from oge::System");
        
        std::size_t idx = typeToIndex_[std::type_index(typeid(T))];
        OGE_ASSERT((idx < systems_.size()), "Tried to get() an uninitialized system from SystemManager");

        // Cast into pointer of child's type and dereference it 
        return *static_cast<T*>(systems_[idx].get());
    }


    /**
     * @brief Updates every system contained in SystemList by calling
     * its onUpdate() method
     * 
     * @param dt const delta time passed to system's onUpdate
    */
    void update(const float dt)
    {
        int i = 0;
        for(auto& system : systems_)
        {
            system->onUpdate(dt);
        }
    }

    /**
     * @brief Updates every system contained in SystemList by calling
     * its onNonFixedUpdate() method
     * 
     * @param dt variable delta time passed to system's onUpdate
    */
    void nonFixedUpdate(const float dt)
    {
        for(auto& system : systems_)
        {
            system->onNonFixedUpdate(dt);
        }
    }

private:
    /** Stores systems inserted into the SystemManager */
    std::vector<system_ptr> systems_;

    /** Maps type_indexes to good indices of systems_ */
    std::unordered_map<std::type_index, size_t> typeToIndex_; 

    /** The Scene this SysteManager belongs to */
    Scene* scene_;
};


} // namespace oge

#include "Scene.h"

#endif // __SYSTEMMANAGER_H__