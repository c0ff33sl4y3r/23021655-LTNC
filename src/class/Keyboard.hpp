#ifndef Keyboard_hpp
#define Keyboard_hpp
#include "Config.cpp"

struct Keyboard{
    Keyboard();
    ~Keyboard();
    void update();
    bool isPressed(SDL_Scancode key);
    bool isReleased(SDL_Scancode key);
    bool isHeld(SDL_Scancode key);
    const Uint8* state;
    Uint8* prevState;
};

#endif