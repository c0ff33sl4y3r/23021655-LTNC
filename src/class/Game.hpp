#ifndef Game_hpp
#define Game_hpp
#include "Sprite.cpp"
#include "Config.cpp"
#include "Map.cpp"
#include "Keyboard.cpp"
#include "Pacman.cpp"
#include "Media.cpp"
struct Game{
    Game(const string &title, int xpos, int ypos, int width, int height, bool fullscreen);
    ~Game();
    void preload();
    void update();
    void render();
    void clean();
    void handleEvents();
    SDL_Renderer* getRenderer(){
        return renderer;
    }
    bool running(){
        return isRunning;
    }
    unordered_map <string, vector <Sprite*>> sprites;
    unordered_map <string, TTF_Font*> fonts;
    bool isRunning;
    int frameCount;
    Keyboard* keyboard;
    Map* map;
    SDL_Window *window;
    SDL_Renderer *renderer;
};
#endif