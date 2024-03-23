#ifndef Text_hpp
#define Text_hpp
#include "Config.cpp"
#include "Render.cpp"
struct UI{
    SDL_Texture* texture;
    SDL_Rect* dstRect;
    bool visible;
    UI();
    UI(SDL_Texture* texture, int x, int y);
    ~UI();
    void render(SDL_Renderer* renderer, int layer);
};

#endif