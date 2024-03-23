#ifndef Animation_cpp
#define Animation_cpp
#include "Animation.hpp"

// Initialize the animation
Animation::Animation(const string &name, SDL_Renderer *renderer, string path, int start, int end, int width, int height, double frameRate, int repeat, int w, int h){
    this->renderer = renderer;
    this->name = name;
    this->currentFrame = 0;
    this->frameRate = frameRate;
    this->loop = 0;
    this->repeat = repeat;
    this->isPlaying = false;
    this->framePerAnim = 1.0 * FPS / frameRate;
    this->width = width;
    this->height = height;
    int col = width / w;
    this->texture = Media::loadTexture(path, renderer);
    for (int i = start - 1; i < end; i++){
        this->positions.push_back(new SDL_Rect{(i % col) * w, (i / col) * h, w, h});
    }
}

//Update the animation
void Animation::update(int currentFrame){
    currentFrame = currentFrame;
    int frameDelta = currentFrame - startFrame;
    int frame = frameDelta / framePerAnim;
    this->currentPos = frame % positions.size();
    // If it's on a new loop
    loop = frame / positions.size();
    if (repeat >= 0 && loop >= repeat){
        isPlaying = false;
        this->currentPos = signed(positions.size() - 1);  // Set the last frame
        return;
    }
}

//Clean the animation
Animation::~Animation(){
    SDL_DestroyTexture(texture);
    positions.clear();
}

// Render
void Animation::render(SDL_Rect* dstRect, double angle, SDL_Point* center, SDL_RendererFlip flip, int layer = 0){
    RenderManager::createRender(renderer, texture, positions[currentPos], dstRect, angle, center, flip, layer);
}
#endif