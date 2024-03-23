#ifndef Layer_cpp
#define Layer_cpp
#include "Layer.hpp"
#include "TileSet.cpp"

Layer::Layer(json* map){
    layerName = (*map)["name"];
    layerType = (*map)["type"];
    layerId = (*map)["id"];
    visible = (*map)["visible"];
    layerOpacity = (*map)["opacity"];
    srcRect = new SDL_Rect();
    srcRect->x = 0;
    srcRect->y = 0;
    if (layerType == "tilelayer"){
        srcRect->w = (*map)["width"];
        srcRect->h = (*map)["height"];
        tileLayer = new TileLayer(map);
    }
    else if (layerType == "objectgroup"){
        objectGroups[layerName] = new ObjectGroup(map);
    }
}

Layer::~Layer(){
    delete srcRect;
    if (layerType == "tilelayer"){
        delete tileLayer;
    }
    else if (layerType == "objectgroup"){
        delete objectGroups[layerName];
    }
    else if (layerType == "imagelayer"){
        delete imageLayer;
    }
}

void Layer::render(TileSet* tileSet, SDL_Renderer* renderer, int layer = 0){
    if (layerType == "tilelayer"){
        tileLayer->render(tileSet, renderer, layer);
    }
}

#endif