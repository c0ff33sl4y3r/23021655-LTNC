#ifndef Text_cpp
#define Text_cpp

#include "Text.hpp"

UI::UI(){
    this->texture = nullptr;
    this->visible = false;
    this->dstRect = new SDL_Rect();
    dstRect->x = 0;
    dstRect->y = 0;
}

UI::UI(SDL_Texture* texture, int x, int y){
    this->texture = texture;
    this->dstRect = new SDL_Rect();
    this->visible = true;
    dstRect->x = x;
    dstRect->y = y;
}

UI::~UI(){
    SDL_DestroyTexture(texture);
    delete dstRect;
}

void UI::render(SDL_Renderer* renderer, int layer = 0){
    if (!visible) return;
    int textWidth, textHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);
    dstRect->w = textWidth;
    dstRect->h = textHeight;
    RenderManager::createRender(renderer, texture, nullptr, dstRect, 0, NULL, SDL_FLIP_NONE, layer);
}

#endif