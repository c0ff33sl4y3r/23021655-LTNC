#ifndef Media_cpp
#define Media_cpp

#include "Media.hpp"

SDL_Texture* Media::loadTexture(const string &file, SDL_Renderer *renderer){
    SDL_Surface* surface = IMG_Load(file.c_str());
    if (surface == nullptr){
        cerr << "Failed to load surface: " << file << endl;
        cerr << IMG_GetError() << endl;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* Media::loadTextureFromRenderedText(const string &text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer){
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr){
        cerr << "Failed to load surface: " << text << endl;
        cerr << TTF_GetError() << endl;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Mix_Chunk* Media::loadSound(const string &file){
    Mix_Chunk* sound = Mix_LoadWAV(file.c_str());
    if (sound == nullptr){
        cerr << "Failed to load sound: " << file << endl;
        cerr << Mix_GetError() << endl;
    }
    return sound;
}

Mix_Music* Media::loadMusic(const string &file){
    Mix_Music* music = Mix_LoadMUS(file.c_str());
    if (music == nullptr){
        cerr << "Failed to load music: " << file << endl;
        cerr << Mix_GetError() << endl;
    }
    return music;
}

TTF_Font* Media::loadFont(const string &file, int size){
    TTF_Font* font = TTF_OpenFont(file.c_str(), size);
    if (font == nullptr){
        cerr << "Failed to load font: " << file << endl;
        cerr << TTF_GetError() << endl;
    }
    return font;
}


#endif