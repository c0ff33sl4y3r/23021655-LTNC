#ifndef Media_hpp
#define Media_hpp
#include "Config.cpp"

struct Media{
    static SDL_Texture* loadTexture(const string &file, SDL_Renderer *renderer);
    static SDL_Texture* loadTextureFromRenderedText(const string &text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer);
    static Mix_Chunk* loadSound(const string &file);
    static Mix_Music* loadMusic(const string &file);
    static TTF_Font* loadFont(const string &file, int size);
};

#endif