#ifndef Sprite_hpp
#define Sprite_hpp
#include "Config.cpp"
#include "Animation.cpp"
#include "Sound.cpp"

struct Sprite{
    Sprite(const string &name, SDL_Renderer *renderer, int x, int y, int w, int h);
    ~Sprite();
    void addAnimation(json config);
    void switchAnimation(string name, int currentFrame);
    void update(int currentFrame);
    SDL_Texture* getTexture();
    void setFlip(SDL_RendererFlip flip){
        this->flip = flip;
    }
    void render();
    bool* visible;
    SDL_Rect* srcRect;
    SDL_Rect* destRect;
    SDL_RendererFlip flip; //Use bitwise operator to flip both horizontally and vertically
    //Sprite Name
    string name;
    //List of animations
    unordered_map <string, Animation*> anims;
    //Current animation
    string currentAnimation;
    string lastAnimation;
    //Renderer
    SDL_Renderer *renderer;
};
#endif