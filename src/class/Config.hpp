#ifndef Config_hpp
#define Config_hpp
#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;
mt19937_64 Rand(chrono::steady_clock::now().time_since_epoch().count());

int SCREEN_WIDTH = 1600;
int SCREEN_HEIGHT = 1024;
const bool FULLSCREEN = 0;
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;
const bool ENABLE_GRAVITY = true;
const int GRAVITY = 0;
bool PAUSED = false;
#endif