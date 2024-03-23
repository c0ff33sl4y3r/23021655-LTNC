#ifndef Animation_hpp
#define Animation_hpp
#include "Config.cpp"
#include "Media.cpp"
#include "Render.cpp"
struct Animation{
    Animation(const string &name, SDL_Renderer *renderer, string path, int start, int end, int width, int height, double frameRate, int repeat, int w, int h);
    ~Animation();
    void play(int currentFrame){
        startFrame = currentFrame;
        isPlaying = true;
    }
    void stop(){
        isPlaying = false;
        loop = 0;
    }
    void render(SDL_Rect* dstRect, double angle, SDL_Point* center, SDL_RendererFlip flip, int layer);
    void update(int currentFrame);
    //List of textures for animations
    vector <SDL_Rect*> positions;
    string name;
    int startFrame;
    int currentFrame;
    int currentPos;
    double frameRate;
    int framePerAnim;
    int loop;
    int repeat;
    int width;
    int height;
    //is it playing? = 0 if loop > repeat;
    bool isPlaying;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
};
#endif