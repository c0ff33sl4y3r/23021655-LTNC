#ifndef Layer_hpp
#define Layer_hpp
#include "Config.cpp"
#include "TileSet.cpp"
#include "TileLayer.cpp"
#include "ObjectGroup.cpp"
#include "ImageLayer.cpp"

// Create a global object Group
unordered_map<string, ObjectGroup*> objectGroups;

struct Layer{
    Layer(json* map);
    void render(TileSet* tileSet, SDL_Renderer* renderer, int layer);
    ~Layer();
    string layerName;
    string layerType; //tilelayer, objectgroup, imagelayer
    SDL_Rect* srcRect;
    int layerId;
    bool visible;
    float layerOpacity;
    TileLayer* tileLayer;
    ImageLayer* imageLayer;
};

#endif