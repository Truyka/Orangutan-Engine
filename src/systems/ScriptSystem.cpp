#include "systems/ScriptSystem.h"
#include "OgeEntity.h"

#include <iostream>

namespace oge
{


void ScriptSystem::onUpdate(const float dt)
{
    // We're copying all the entities that have a Script attached because new 
    // may be added during iteration and it would make it unpredictable
    std::vector<aecs::Entity> copy = scene_->view<ScriptList>().getInnerContainer();

    // This will loop through every entity's scriptlist, 
    // instantiate every non-valid script and delete the detached ones
    for(size_t idx = 0; idx < copy.size(); idx++)
    {
        auto entity = copy[idx];

        // Entities in the internal cointainer may be invalid, so we need to check for it
        // TODO: Make an aecs function to make a copy of internal container with valid entities
        if(!entity.isValid()) continue;

        ScriptList& sl = scene_->get<ScriptList>(entity);

        auto& sc = sl.scripts;
        for(size_t i = 0; i < sc.size();)
        {
            if(sc[i]->valid() == false)
            {
                Entity ent(entity, scene_);     
                sc[i]->instatiate(ent, &sl);
                sc[i]->onAttach();
            }

            sc[i]->onUpdate(dt);

            if(sc[i]->isRemoved())
            {
                sc[i]->onDetach();
                sc.erase(sc.begin() + i);
            }
            else 
                ++i;
        }
    }
}

void ScriptSystem::onNonFixedUpdate(const float dt)
{
    auto view = scene_->view<ScriptList>();
    view.each([&](ScriptList& sl)
    {
        // Get the reference to ScriptList's internal scripts storage
        auto& scripts = sl.scripts;

        for(auto it = scripts.begin(); it != scripts.end(); ++it)
        {
            auto& sc = *it;
            if(sc->valid())
            {
                sc->onNonFixedUpdate(dt);
            }
        }
    });
}


} // namespace oge