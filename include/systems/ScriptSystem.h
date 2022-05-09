#ifndef __SCRIPTSYSTEM_H__
#define __SCRIPTSYSTEM_H__

#include "components/Script.h"

#include "SystemManager.h"
#include "Config.h"

namespace oge
{


class ScriptSystem : public System
{
public:
    OGE_SPECIFY_CLASS_NAME("ScriptSystem")

    /**
     * @brief Initializes every uninitialized script, removes any
     * detached script and calls onUpdate of every script
     * 
     * @param dt a const value specifying the game's const framerate
    */
    virtual void onUpdate(const float dt) override;

    /**
     * @brief Calls onNonFixedUpdate of every valid script. Note
     * that it doesn't initialize or remove scripts. For that aspect,
     * it relies on normal, fixed updates 
     * 
     * @param dt a variable delta time telling how much time the previous
     * frame took in seconds
    */
    virtual void onNonFixedUpdate(const float dt) override;
};


} // namespace oge
#endif // __SCRIPTSYSTEM_H__