#ifndef Render_hpp
#define Render_hpp
#include "Config.cpp"

struct Render{
    Render(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, double angle, SDL_Point* center, SDL_RendererFlip flip, int layer, int order);
    ~Render();
    void render();
    bool operator < (const Render& other) const{
        if (layer != other.layer) return layer < other.layer;
        if (dstRect->y + dstRect->h == other.dstRect->y + other.dstRect->h){
            return order < other.order;
            if (dstRect->x + dstRect->w == other.dstRect->x + other.dstRect->w)
                return order < other.order;
            return dstRect->x + dstRect->w < other.dstRect->x + other.dstRect->w;
        }
        return dstRect->y + dstRect->h < other.dstRect->y + other.dstRect->h;
    }
    int layer;
    int order;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
    double angle;
    SDL_Point* center;
    SDL_RendererFlip flip;
};

struct RenderManager{
    RenderManager();
    ~RenderManager();
    static void render();
    static void createRender(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, double angle, SDL_Point* center, SDL_RendererFlip flip, int layer);
    static vector <Render*> objects;
};

#endif