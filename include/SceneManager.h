#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Scene.h"

namespace oge
{


class SceneManager
{
public:
    using scene_ptr = std::unique_ptr<Scene>;

public:
    SceneManager() : currentSceneIdx_(SIZE_MAX)
    {

    }

    /**
     * @brief Constructs a new scene and appends it to the 
     * scene list. Sets the scene's member pointer 'manager_'
     * to the Manager it was initialized in
     * 
     * @param args arguments passed to the scene ctor 
    */
    template<typename T, typename... Args>
    void add(Args&&... args)
    {
        static_assert(std::is_base_of_v<Scene, T>, "The type you're has to be derived from oge::Scene");

        OGE_LOG_INFO("Adding a new scene to the SceneList: " << OGE_GET_CLASS_NAME(T));

        std::type_index typeidx(typeid(T));
        if(typeToIndex_.find(typeidx) != typeToIndex_.end()) // if it already exists
        {
            OGE_LOG_WARNING("SceneManager already has a system called: " << OGE_GET_CLASS_NAME(T));
            return;
        }

        scene_ptr ptr = std::make_unique<T>(std::forward<Args>(args)...);
        ptr->manager_ = this;

        typeToIndex_[typeidx] = scenes_.size();
        scenes_.push_back(std::move(ptr));

        // If no scene is seleceted change to the newly added one
        if(currentSceneIdx_ == SIZE_MAX)
            changeTo<T>();
    }

    /**
     * @brief Gets a scene of specified type
     * 
     * @return T& reference to the scene
    */
    template<typename T>
    T& get()
    {
        static_assert(std::is_base_of_v<Scene, T>, "The type you're getting has to be derived from oge::Scene");

        std::size_t idx = typeToIndex_[std::type_index(typeid(T))];
        OGE_ASSERT((idx < scenes_.size()), "Tried to get() an uninitialized state from StateManager");

        // Cast into pointer of child's type and dereference it 
        return *static_cast<T*>(scenes_[idx].get());
    }

    /**
     * @brief Changes the current scene to T
     * 
     * @tparam T 
    */
    template<typename T>
    void changeTo()
    {
        static_assert(std::is_base_of_v<Scene, T>, "The type you're changing to has to be derived from oge::Scene");

        OGE_LOG_INFO("Changing current state to " << OGE_GET_CLASS_NAME(T));
        std::type_index typeidx(typeid(T));

        auto ttipair = typeToIndex_.find(typeidx);
        if(ttipair != typeToIndex_.end()) // if it already exists
        {
            if(currentSceneIdx_ < scenes_.size())
            {
                Scene* currentScene = scenes_[currentSceneIdx_].get();

                if(currentScene == scenes_[ttipair->second].get())
                {
                    OGE_LOG_WARNING("Current state is already: " << OGE_GET_CLASS_NAME(T));
                    return;
                }

                if(currentScene != nullptr)
                {
                    currentScene->onExit();
                }
            }

            currentSceneIdx_ = ttipair->second;
            scenes_[currentSceneIdx_]->onEnter();
        }
        else
        {
            OGE_LOG_WARNING("A state with this name doesn't exist: " << OGE_GET_CLASS_NAME(T));
        }
    }

    Scene& current()
    {
        if(currentSceneIdx_ == SIZE_MAX)
        {
            OGE_LOG_ERROR("Scene manager current() called when `currentSceneIdx_ == SIZE_MAX`")
            throw std::runtime_error("Call to `current` resulted in memory access violation - no scenes were added to the game");
        }
        return *scenes_[currentSceneIdx_];
    }

private:
    /** Points to the current scene */
    size_t currentSceneIdx_;

    /** Stores systems inserted into the SystemManager */
    std::vector<scene_ptr> scenes_;

    /** Maps type_indexes to good indices of systems_ */
    std::unordered_map<std::type_index, size_t> typeToIndex_; 
};

/* This function's implemenation needs to be defined here
 * because it has to know SceneManager's definition to work */
template<typename T>
void Scene::changeTo()
{
    manager_->changeTo<T>();
}


} // namespcae oge
#endif // __SCENEMANAGER_H__