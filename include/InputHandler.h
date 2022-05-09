#ifndef __INPUTHANDLER_H__
#define __INPUTHANDLER_H__

#include "Config.h"
#include "math/Vector2f.h"

#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <SDL.h>

namespace oge
{


class InputHandler
{
public:
    OGE_SPECIFY_CLASS_NAME("InputHandler")
    OGE_DELETE_ASSIGN(InputHandler)

    /** @brief Accesses the static instance of InputHandler */
    static InputHandler& instance();

public:
    void update();

    bool keyDown(SDL_Keycode kcode);
    bool keyUp(SDL_Keycode kcode);
    bool keyPressed(SDL_Keycode kcode);
    bool keyUnpressed(SDL_Keycode kcode);

    Vector2f getMousePos();

    bool quit();

private:
    InputHandler();

    bool quit_;
    const uint8_t* keyboard_;
    std::unordered_map<SDL_Keycode, bool> keysPressed_;
    std::unordered_map<SDL_Keycode, bool> keysUnpressed_;
};

extern InputHandler& input;


} // namespace oge
#endif // __INPUTHANDLER_H__