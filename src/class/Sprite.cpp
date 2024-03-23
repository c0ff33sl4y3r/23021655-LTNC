#ifndef Sprite_cpp
#define Sprite_cpp
#include "Sprite.hpp"

// Initialize the sprite
Sprite::Sprite(const string &name, SDL_Renderer *renderer, int x, int y, int w, int h){
    this->name = name;
    this->renderer = renderer;
    visible = new bool(true);
    srcRect = new SDL_Rect();
    destRect = new SDL_Rect();
    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = w;
    srcRect->h = h;
    destRect->w = w * 2;
    destRect->h = h * 2;
    if (name == "Player"){
        destRect->x = x - w;
        destRect->y = y - 2 * h;
    }
    else if (name == "Coin" || name == "Diamond" || name == "Bonus"){
        destRect->x = x - w;
        destRect->y = y - h;
    }
    else{
        destRect->x = x - w;
        destRect->y = y - h - h / 2;
    }
    currentAnimation = lastAnimation = "";
    flip = SDL_FLIP_NONE;
    cerr << "Sprite created: " << this->name << "\n";
}

// Add an animation to the sprite
void Sprite::addAnimation(json config){
    string name = config["name"];
    string path = config["path"];
    int start = config["start"];
    int end = config["end"];
    int width = config["width"];
    int height = config["height"];
    double frameRate = config["rate"];
    int repeat = config["repeat"];
    Animation *animation = new Animation(name, renderer, path, start, end, width, height, frameRate, repeat, srcRect->w, srcRect->h);
    anims[name] = animation;
    if (currentAnimation == ""){
        currentAnimation = name;
        lastAnimation = name;
    }
}

// Switch the current animation
void Sprite::switchAnimation(string name, int currentFrame){
    if (currentAnimation == name){
        return;
    }
    if (anims.find(currentAnimation) != anims.end()){
        anims[currentAnimation]->stop();
    }
    lastAnimation = currentAnimation;
    currentAnimation = name;
    if (anims.find(currentAnimation) != anims.end()){
        anims[currentAnimation]->play(currentFrame);
    }
}

// Update the sprite animation
void Sprite::update(int currentFrame){
    if (currentAnimation == ""){
        cerr << "No animation to update\n";
        return;
    }
    if (anims.find(currentAnimation) != anims.end()){
        anims[currentAnimation]->update(currentFrame);
    }
}

// Get the texture
SDL_Texture* Sprite::getTexture(){
    if (anims.find(currentAnimation) != anims.end()){
        return anims[currentAnimation]->texture;
    }
    return NULL;
}

// Destructor
Sprite::~Sprite(){
    delete srcRect;
    delete destRect;
    for (auto &anim : anims){
        delete anim.second;
    }
}

// Render the sprite
void Sprite::render(){
    if (!(*visible)){
        return;
    }
    if (anims.find(currentAnimation) != anims.end()){
        int layer = 3;
        if (name == "Coin" || name == "Diamond" || name == "Bonus"){
            layer = 0;
        }
        anims[currentAnimation]->render(destRect, 0, NULL, flip, layer);
    }
}
#endif