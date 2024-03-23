#ifndef Keyboard_cpp
#define Keyboard_cpp
#include "Keyboard.hpp"

Keyboard::Keyboard(){
    state = SDL_GetKeyboardState(NULL);
    prevState = new Uint8[SDL_NUM_SCANCODES];
    for (int i = 0; i < SDL_NUM_SCANCODES; i++){
        prevState[i] = state[i];
    }
}

Keyboard::~Keyboard(){
    delete[] prevState;
}

void Keyboard::update(){
    for (int i = 0; i < SDL_NUM_SCANCODES; i++){
        prevState[i] = state[i];
    }
    state = SDL_GetKeyboardState(NULL);
}

bool Keyboard::isPressed(SDL_Scancode key){
    return state[key] && !prevState[key];
}

bool Keyboard::isReleased(SDL_Scancode key){
    return !state[key] && prevState[key];
}

bool Keyboard::isHeld(SDL_Scancode key){
    return state[key];
}
#endif