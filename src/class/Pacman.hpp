#ifndef Pacman_hpp
#define Pacman_hpp
#include "Config.cpp"
#include "Sprite.cpp"
#include "Map.cpp"
#include "Text.cpp"
#include "Sound.cpp"

int dx[] = {0, -1, 0, 1};
int dy[] = {-1, 0, 1, 0};

struct Pacman{
    static Sprite* sprite;
    static unordered_map <string, vector <Sprite*>> *sprites;
    static int speed; //pixel per second
    static int lives;
    static int score;
    // 0: up 1: left 2: down 3: right
    static int direction; // Current direction
    static int pendingDirection; // Next direction
    static int powerTime; // Power pellet time
    static int pelletKill; // Number of ghosts killed by power pellet
    static int counting; // If the game is counting down
    static int eaten; // Counting the number of eaten dots
    static bool winning;
    static void Precal(); // Precalculate
    static void Init(unordered_map <string, vector <Sprite*>>* sprites, TileLayer* tileLayer, TileSet* tileSet, TTF_Font* font); // Initialize the game
    static void update(int frameCount); // Compute next move
    static void render(); // Render the sprite
    static int getX(){
        return sprite->destRect->x + sprite->destRect->w / 2;
    }
    static int getY(){
        return sprite->destRect->y + sprite->destRect->h;
    }
    static TileLayer* tileLayer;
    static TileSet* tileSet;
    static vector <vector <int>> move;
    static vector <Sprite*> *coins;
    static vector <Sprite*> *diamonds;
    static vector <Sprite*> *bonus;
    static TTF_Font* font;
    static void reset();
};

struct Blinky{
    static Sprite* sprite;
    static int speed;
    static int direction;
    static bool isDead;
    static bool spawning;
    static bool scatter;
    static bool frightened;
    static void getDirection(int s);
    static void reset();
};

struct Pinky{
    static Sprite* sprite;
    static int speed;
    static int direction;
    static bool isDead;
    static bool spawning;
    static bool scatter;
    static bool frightened;
    static void getDirection(int s);
    static void reset();
};

struct Inky{
    static Sprite* sprite;
    static int speed;
    static int direction;
    static bool isDead;
    static bool spawning;
    static bool scatter;
    static bool frightened;
    static void getDirection(int s);
    static void reset();
};

struct Clyde{
    static Sprite* sprite;
    static int speed;
    static int direction;
    static bool isDead;
    static bool spawning;
    static bool scatter;
    static bool frightened;
    static void getDirection(int s);
    static void reset();
};

struct Ghost{
    template <class T> static int getX(){
        return T::sprite->destRect->x + T::sprite->destRect->w / 2;
    }
    template <class T> static int getY(){
        return T::sprite->destRect->y + T::sprite->destRect->h / 2 + T::sprite->destRect->h / 4;
    }
    template <class T> static void update(int frameCount);
    template <class T> static bool collide(); // Check if collide with pacman
    template <class T> static void render();
    template <class T> static void reset();
};

#endif