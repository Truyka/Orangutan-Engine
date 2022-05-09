#include "InputHandler.h"
#include "Config.h"

namespace oge
{


InputHandler& input = InputHandler::instance();

InputHandler::InputHandler() : keyboard_(nullptr), quit_(false)
{
    
}

InputHandler& InputHandler::instance()
{
    static InputHandler input;
    return input;
}

void InputHandler::update()
{
    keyboard_ = SDL_GetKeyboardState(nullptr);
    keysPressed_.clear(); 
    keysUnpressed_.clear(); 

    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        SDL_Keycode keycode = e.key.keysym.sym;
        if(e.type == SDL_QUIT)
        {
            quit_ = true;
        }
        else if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            keysPressed_[keycode] = true;
        }
        else if(e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            keysUnpressed_[keycode] = true;
        }
    }
}

bool InputHandler::keyDown(SDL_Keycode kcode)
{
    if(keyboard_ == nullptr) return false;
    return keyboard_[SDL_GetScancodeFromKey(kcode)];
}

bool InputHandler::keyUp(SDL_Keycode kcode)
{
    return !keyDown(kcode);
}

bool InputHandler::keyPressed(SDL_Keycode kcode)
{
    return keysPressed_[kcode];
}

bool InputHandler::keyUnpressed(SDL_Keycode kcode) 
{
    return keysUnpressed_[kcode];
}

Vector2f InputHandler::getMousePos()
{
    int x = 0, y = 0;
    SDL_GetMouseState(&x, &y);
    return Vector2f(x, y);
}

bool InputHandler::quit()
{
    return quit_;
}


} // namespace oge