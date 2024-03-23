#ifndef Tilecpp
#define Tilecpp
#include "Tile.hpp"
#include "Render.cpp"

Tile::Tile(SDL_Texture* texture, SDL_Rect* srcRect, json properties = json::parse(R"###([])###")){
    this->texture = texture;
    this->srcRect = srcRect;
    this->properties = properties;
}

Tile::~Tile(){
    SDL_DestroyTexture(texture);
    delete srcRect;
}

int cnttt = 0;
void Tile::render(SDL_Renderer* renderer, int i, int j, int layer = 0) {
    SDL_Rect destRect = getRect(i, j);
    //SDL_RenderCopyEx(renderer, texture, srcRect, destRect, 0, NULL, SDL_FLIP_NONE);
    RenderManager::createRender(renderer, texture, srcRect, &destRect, 0, NULL, SDL_FLIP_NONE, layer);
}

SDL_Rect Tile::getRect(int i, int j){
    SDL_Rect destRect = SDL_Rect{j * srcRect->w, i * srcRect->h, srcRect->w, srcRect->h};
    return destRect;
}


#endif