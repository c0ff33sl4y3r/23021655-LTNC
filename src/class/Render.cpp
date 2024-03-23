#ifndef Render_cpp
#define Render_cpp
#include "Render.hpp"


vector <Render*> RenderManager::objects;
Render::Render(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, double angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int layer = 0, int order = 0){
    this->renderer = renderer;
    this->texture = texture;
    this->srcRect = srcRect;
    this->dstRect = dstRect;
    this->angle = angle;
    this->center = center;
    this->flip = flip;
    this->layer = layer;
    this->order = order;
}

Render::~Render(){
    SDL_DestroyTexture(texture);
    delete srcRect;
    delete dstRect;
    delete center;
}

void Render::render(){
    SDL_RenderCopyEx(renderer, texture, srcRect, dstRect, angle, center, flip);
}

RenderManager::RenderManager(){}
RenderManager::~RenderManager(){}

void RenderManager::render(){
    sort(objects.begin(), objects.end(),[](const Render* i, const Render* j){
        return *i < *j;
    });
    for (auto object: objects){
        object->render();
    }
    objects.clear();
}

void RenderManager::createRender(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, double angle, SDL_Point* center, SDL_RendererFlip flip, int layer = 0){
    SDL_Rect* dstRectCopy = new SDL_Rect(*dstRect);
    objects.push_back(new Render(renderer, texture, srcRect, dstRectCopy, angle, center, flip, layer, objects.size()));
}

#endif