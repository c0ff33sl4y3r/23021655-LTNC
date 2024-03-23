#include "Game.cpp"
#include "Config.hpp"
#include <bits/stdc++.h>
#include <SDL2/SDL.h>
using namespace std;

// Create new game variable
Game* game;

int main(int argc, char *args[])
{
    // Initialize the game
    if (FULLSCREEN){
        SCREEN_WIDTH = 1920;
        SCREEN_HEIGHT = 1080;
    }
    game = new Game("Arcade Adventure", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN);
    game->preload();
    int frameDelay = 1000 / FPS;
    int frameStart = 0;
    int frameTime = 0;
    // Run until the game is not running
    while (game->running()){
        frameStart = SDL_GetTicks();
        game->handleEvents();
        if (!PAUSED){
            game->update();
            game->render();
        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }
    // Clean the game
    game->clean();
    return 0;
}