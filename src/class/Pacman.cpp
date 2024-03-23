#ifndef Pacman_cpp
#define Pacman_cpp
#include "Pacman.hpp"

Sprite* Pacman::sprite;
Sprite* Blinky::sprite;
Sprite* Pinky::sprite;
Sprite* Inky::sprite;
Sprite* Clyde::sprite;
unordered_map <string, vector <Sprite*>> *Pacman::sprites;
int Pacman::speed;
int Pacman::lives;
int Pacman::score;
int Pacman::direction;
int Pacman::pendingDirection;
int Pacman::powerTime;
int Pacman::pelletKill;
int Pacman::counting;
int Pacman::eaten;
bool Pacman::winning;
vector <Sprite*> *Pacman::coins;
vector <Sprite*> *Pacman::diamonds;
vector <Sprite*> *Pacman::bonus;
TileSet* Pacman::tileSet;
TileLayer* Pacman::tileLayer;
TTF_Font* Pacman::font;
vector <vector <int>> Pacman::move;


int Blinky::speed;
int Blinky::direction;
bool Blinky::isDead;
bool Blinky::spawning;
bool Blinky::scatter;
bool Blinky::frightened;

int Pinky::speed;
int Pinky::direction;
bool Pinky::isDead;
bool Pinky::spawning;
bool Pinky::scatter;
bool Pinky::frightened;

int Inky::speed;
int Inky::direction;
bool Inky::isDead;
bool Inky::spawning;
bool Inky::scatter;
bool Inky::frightened;

int Clyde::speed;
int Clyde::direction;
bool Clyde::isDead;
bool Clyde::spawning;
bool Clyde::scatter;
bool Clyde::frightened;

unordered_map <string, UI*> textObj;

void Pacman::Precal(){
    move.resize(tileLayer->layerHeight, vector <int>(tileLayer->layerWidth, 0));
    // The first line is ignored
    // The wall is ignored
    for (int i = 2; i < tileLayer->layerHeight - 1; ++i){
        for (int j = 1; j < tileLayer->layerWidth - 1; ++j){
            //  up  left down right
            //   1   1     1    1
            //   0   1     2    3
            move[i - 1][j] |= (tileLayer->layerData[i - 1][j] == 0) << 0;
            move[i - 1][j] |= (tileLayer->layerData[i][j - 1] == 0) << 1;
            move[i - 1][j] |= (tileLayer->layerData[i + 1][j] == 0) << 2;
            move[i - 1][j] |= (tileLayer->layerData[i][j + 1] == 0) << 3;
            if (tileLayer->layerData[i][j]) move[i - 1][j] = 0;
        }
    }
    move[14][0] |= 1 << 3;
    move[14][28] |= 1 << 1;
}

void Pacman::Init(unordered_map <string, vector <Sprite*>>* sprites, TileLayer* tileLayer, TileSet* tileSet, TTF_Font* font){
    Pacman::sprites = sprites;
    Pacman::sprite = (*sprites)["Player"][0];
    Blinky::sprite = (*sprites)["Fox"][0];
    Pinky::sprite = (*sprites)["Racoon"][0];
    Inky::sprite = (*sprites)["Bird"][0];
    Clyde::sprite = (*sprites)["Cat"][0];
    Pacman::tileLayer = tileLayer;
    Pacman::tileSet = tileSet;
    Pacman::pelletKill = 0;
    Pacman::coins = &(*sprites)["Coin"];
    Pacman::diamonds = &(*sprites)["Diamond"];
    Pacman::font = font;
    textObj["Score:"] = new UI(nullptr, 950, 400);
    textObj["Score"] = new UI(nullptr, 950, 500);
    textObj["Lives:"] = new UI(nullptr, 950, 600);
    textObj["Lives"] = new UI(nullptr, 1150, 600);
    textObj["Score:"]->texture = Media::loadTextureFromRenderedText("Score:", {255, 255, 255}, font, sprite->renderer);
    textObj["Lives:"]->texture = Media::loadTextureFromRenderedText("Lives:", {255, 255, 255}, font, sprite->renderer);
    
    // Set game properties;
    Pacman::lives = 3;
    // 300 eaten to win
    Pacman::eaten = 0;
    Pacman::winning = false;

    // Precalculate
    Pacman::Precal();

    Pacman::reset();

    (*sprites)["Target"][0]->visible = &Blinky::frightened;
    (*sprites)["Target"][1]->visible = &Pinky::frightened;
    (*sprites)["Target"][2]->visible = &Inky::frightened;
    (*sprites)["Target"][3]->visible = &Clyde::frightened;
}   

void Pacman::reset(){
    // Pacman reset
    
    Pacman::counting = 3000;
    Pacman::sprite->destRect->x = objectGroups["Spawn"]->objectsByName["Player"][0]->objectX;
    Pacman::sprite->destRect->y = objectGroups["Spawn"]->objectsByName["Player"][0]->objectY;
    Pacman::sprite->destRect->x = 2 * Pacman::sprite->destRect->x - Pacman::getX();
    Pacman::sprite->destRect->y = 2 * Pacman::sprite->destRect->y - Pacman::getY();
    Pacman::sprite->switchAnimation("RunH", 0);
    Pacman::sprite->setFlip(SDL_FLIP_NONE);
    Pacman::direction = 3;
    Pacman::pendingDirection = 3;
    Pacman::speed = 180 / FPS;
    Pacman::powerTime = 0;

    // Ghost reset
    Blinky::direction = 3;
    Pinky::direction = 0;
    Inky::direction = 3;
    Clyde::direction = 1;
    Ghost::reset<Blinky>();
    Ghost::reset<Pinky>();
    Ghost::reset<Inky>();
    Ghost::reset<Clyde>();

}

void Pacman::update(int frameCount){
    if (counting > 0){
        counting -= 1000 / FPS;
        return;
    }

    int x1 = (Pacman::sprite->destRect->x + 16) / 32;
    int y1 = (Pacman::sprite->destRect->y + 48) / 32;
    int x2 = (Pacman::sprite->destRect->x + 16 + 31) / 32;
    int y2 = (Pacman::sprite->destRect->y + 48 + 31) / 32;

    for (int i = signed(coins->size()) - 1; i >= 0; --i){
        auto coin = (*coins)[i];
        int x = (coin->destRect->x + coin->destRect->w - 16) / 32;
        int y = (coin->destRect->y + coin->destRect->h - 16) / 32;
        if ((x == x1 && y == y1) || (x == x2 && y == y2)){
            Pacman::score += 10;
            ++eaten;
            SoundManager::playSound("coin.collect");
            swap((*coins)[i], coins->back());
            coins->pop_back();
        }
    }

    for (int i = signed(diamonds->size()) - 1; i >= 0; --i){
        auto diamond = (*diamonds)[i];
        int x = (diamond->destRect->x + diamond->destRect->w - 16) / 32;
        int y = (diamond->destRect->y + diamond->destRect->h - 16) / 32;
        if ((x == x1 && y == y1) || (x == x2 && y == y2)){
            Pacman::score += 50;
            ++eaten;
            powerTime = 10000;
            SoundManager::playSound("diamond.collect");
            Blinky::frightened = Pinky::frightened = Inky::frightened = Clyde::frightened = true;
            pelletKill = 0;
            swap((*diamonds)[i], diamonds->back());
            diamonds->pop_back();
        }
    }

    if (eaten == 300 && !winning){
        textObj["Lives:"]->texture = Media::loadTextureFromRenderedText("You Win", {0, 255, 0}, font, sprite->renderer);
        SoundManager::playSound("diamond.collect");
        winning = true;
    }
    
    if (powerTime - 1000 / FPS <= 0){
        Blinky::frightened = Pinky::frightened = Inky::frightened = Clyde::frightened = false;
    }
    powerTime -= 1000 / FPS;
    if (pendingDirection % 2 == direction % 2){
        direction = pendingDirection;
    }
    // If it's not then it's a turn, so check if it's turnable on the next move
    // Get pixel needed to go to the crossroads
    int x = sprite->destRect->x + 16;
    int y = sprite->destRect->y + 48;
    if (direction < 2){
        x = - x % 32;
        y = - y % 32;
    }
    else{
        x = (32 - x % 32) % 32;
        y = (32 - y % 32) % 32;
    }
    int cnt = speed;
    if (abs(x) + abs(y) < cnt){
        sprite->destRect->x += x;
        sprite->destRect->y += y;
        cnt -= abs(x) + abs(y);
        int tileX = (sprite->destRect->x + 16) / 32;
        int tileY = (sprite->destRect->y + 48) / 32;
        if (tileX == 28 && direction == 3) sprite->destRect->x -= 28 * 32;
        if (tileX == 0 && direction == 1) sprite->destRect->x += 28 * 32;
        if (move[tileY - 1][tileX] & (1 << pendingDirection)){
            direction = pendingDirection;
        }
        if (!(move[tileY - 1][tileX] & (1 << direction))){
            cnt = 0;
        }
    }
    if (cnt){
        bool flag = 0;
        for (int i = 0; i < 6; ++i){
            if (SoundManager::isPlayingSound("player.move" + to_string(i))){
                flag = 1;
                break;
            }
        }
        if (!flag){
            SoundManager::playSound("player.move" + to_string(rd(1, 6)));
        }
        sprite->destRect->x += cnt * dx[direction];
        sprite->destRect->y += cnt * dy[direction];
    }

    // Update ghost to move
    Ghost::update<Blinky>(frameCount);
    Ghost::update<Pinky>(frameCount);
    Ghost::update<Inky>(frameCount);
    Ghost::update<Clyde>(frameCount);

    // Check if the pacman loses a life

    if (lives <= 0){
        lives = -lives;
        if (lives == 0){
            textObj["Lives:"]->texture = Media::loadTextureFromRenderedText("Game Over", {255, 0, 0}, font, sprite->renderer);
        }
    }
    if (Pacman::winning){
        if (rd(1, 10) == 1){
            SoundManager::playSound("firework.launch");
        }
    }
    for (const string& name: SoundManager::expires){
        if (name == "firework.launch"){
            SoundManager::playSound("firework.explode" + to_string(rd(1, 2)));
        }
    }
}

void Blinky::getDirection(int mask){
    pair <int, int> ans = make_pair(1e9, -1);
    for (int i = 0; i < 4; ++i){
        if ((mask & (1 << i)) == 0) continue;
        if (i == (direction + 2) % 4) continue;
        int x = Ghost::getX<Blinky>() + dx[i];
        int y = Ghost::getY<Blinky>() + dy[i];
        int a;
        if (isDead){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY);
            if (a < 128 * 128){
                isDead = false;
                speed = 120 / FPS;
                direction = -1;
                spawning = true;
                sprite->destRect->x = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX;
                sprite->destRect->y = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY;
                sprite->destRect->x = 2 * sprite->destRect->x - Ghost::getX<Blinky>();
                sprite->destRect->y = 2 * sprite->destRect->y - Ghost::getY<Blinky>();
                return;
            }
        }
        else if (scatter){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Fox Scatter"][0]->objectX, objectGroups["Spawn"]->objectsByName["Fox Scatter"][0]->objectY);
        }
        else{
            a = dist(x, y, Pacman::getX(), Pacman::getY());
        }
        ans = min(ans, make_pair(a, i));
    }
    direction = ans.second;
}

void Pinky::getDirection(int mask){
    pair <int, int> ans = make_pair(1e9, -1);
    for (int i = 0; i < 4; ++i){
        if ((mask & (1 << i)) == 0) continue;
        if (i == (direction + 2) % 4) continue;
        int x = Ghost::getX<Pinky>() + dx[i];
        int y = Ghost::getY<Pinky>() + dy[i];
        int a;
        if (isDead){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY);
            if (a < 128 * 128){
                isDead = false;
                speed = 120 / FPS;
                direction = -1;
                spawning = true;
                sprite->destRect->x = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX;
                sprite->destRect->y = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY;
                sprite->destRect->x = 2 * sprite->destRect->x - Ghost::getX<Pinky>();
                sprite->destRect->y = 2 * sprite->destRect->y - Ghost::getY<Pinky>();
                return;
            }
        }
        else if (scatter){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Racoon Scatter"][0]->objectX, objectGroups["Spawn"]->objectsByName["Racoon Scatter"][0]->objectY);
        }
        else{
            int d = Pacman::direction;
            a = dist(x, y, Pacman::getX() + dx[d] * 128, Pacman::getY() + dy[d] * 128);
        }
        ans = min(ans, make_pair(a, i));
    }
    direction = ans.second;
}

void Inky::getDirection(int mask){
    pair <int, int> ans = make_pair(1e9, -1);
    for (int i = 0; i < 4; ++i){
        if ((mask & (1 << i)) == 0) continue;
        if (i == (direction + 2) % 4) continue;
        int x = Ghost::getX<Inky>() + dx[i];
        int y = Ghost::getY<Inky>() + dy[i];
        int a;
        if (isDead){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY);
            if (a < 128 * 128){
                isDead = false;
                speed = 120 / FPS;
                spawning = true;
                direction = -1;
                sprite->destRect->x = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX;
                sprite->destRect->y = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY;
                sprite->destRect->x = 2 * sprite->destRect->x - Ghost::getX<Inky>();
                sprite->destRect->y = 2 * sprite->destRect->y - Ghost::getY<Inky>();
                return;
            }
        }
        else if (scatter){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Bird Scatter"][0]->objectX, objectGroups["Spawn"]->objectsByName["Bird Scatter"][0]->objectY);
        }
        else{
            int x1 = Ghost::getX<Blinky>();
            int y1 = Ghost::getY<Blinky>();
            int d = Pacman::direction;
            int x2 = Pacman::getX() + dx[d] * 64;
            int y2 = Pacman::getY() + dy[d] * 64;
            a = dist(x, y, x2 + x2 - x1, y2 + y2 - y1);
        }
        ans = min(ans, make_pair(a, i));
    }
    direction = ans.second;
}

void Clyde::getDirection(int mask){
    pair <int, int> ans = make_pair(1e9, -1);
    if (dist(Ghost::getX<Clyde>(), Ghost::getY<Clyde>(), Pacman::getX(), Pacman::getY()) < 256 * 256){
        scatter = 1;
    }
    for (int i = 0; i < 4; ++i){
        if ((mask & (1 << i)) == 0) continue;
        if (i == (direction + 2) % 4) continue;
        int x = Ghost::getX<Clyde>() + dx[i];
        int y = Ghost::getY<Clyde>() + dy[i];
        int a;
        if (isDead){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX, objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY);
            if (a < 128 * 128){
                isDead = false;
                speed = 180 / FPS;
                spawning = true;
                direction = -1;
                sprite->destRect->x = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectX;
                sprite->destRect->y = objectGroups["Spawn"]->objectsByName["Racoon"][0]->objectY;
                sprite->destRect->x = 2 * sprite->destRect->x - Ghost::getX<Clyde>();
                sprite->destRect->y = 2 * sprite->destRect->y - Ghost::getY<Clyde>();
                return;
            }
        }
        else if (scatter){
            a = dist(x, y, objectGroups["Spawn"]->objectsByName["Cat Scatter"][0]->objectX, objectGroups["Spawn"]->objectsByName["Cat Scatter"][0]->objectY);
        }
        else{
            a = dist(x, y, Pacman::getX(), Pacman::getY());
        }
        ans = min(ans, make_pair(a, i));
    }
    direction = ans.second;
}

template <class T> void Ghost::update(int frameCount){
    if (Pacman::sprite->currentAnimation == "Hurt" || Pacman::sprite->currentAnimation == "Death" || Pacman::winning) return;
    int x = getX<T>();
    int y = getY<T>();
    if (T::spawning){
        if (x == 448){
            if (y - T::speed < 400){
                T::sprite->destRect->y -= y - 400;
                T::spawning = false;
                T::speed = 180 / FPS;
                int a = dist(x + dx[1], y + dy[1], Pacman::getX(), Pacman::getY());
                int b = dist(x + dx[3], y + dy[3], Pacman::getX(), Pacman::getY());
                if (a < b){
                    T::direction = 1;
                }
                else{
                    T::direction = 3;
                }
            }
            else{
                T::sprite->destRect->y -= T::speed;
            }
        }
        else{
            if (abs(x - 448) < T::speed){
                T::sprite->destRect->x += 448 - x;
                T::direction = 0;
            }
            else{
                T::sprite->destRect->x += dx[T::direction] * T::speed;
            }
        }
    }
    else{
        x = - (x - 16) % 32;
        y = - (y - 16) % 32;
        if (T::direction > 1){
            x = (32 + x) % 32;
            y = (32 + y) % 32;
        }
        int cnt = T::speed;
        if (abs(x) + abs(y) < cnt){
            cnt -= abs(x) + abs(y);
            T::sprite->destRect->x += x;
            T::sprite->destRect->y += y;
            int tileX = (getX<T>() - 16) / 32;
            int tileY = (getY<T>() - 16) / 32;
            if (tileX == 28 && T::direction == 3){
                T::sprite->destRect->x -= 28 * 32;
                tileX = 0;
            }
            if (tileX == 0 && T::direction == 1){
                T::sprite->destRect->x += 28 * 32;
                tileX = 28;
            }
            if (T::frightened){
                T::speed = 120 / FPS;
                if ((Pacman::move[tileY - 1][tileX] & (1 << T::direction)) == 0){
                    vector <int> can;
                    for (int i = 0; i < 4; ++i){
                        if (Pacman::move[tileY - 1][tileX] & (1 << i)){
                            if (i == (T::direction + 2) % 4) continue;
                            can.push_back(i);
                        }
                    }
                    T::direction = can[rd(0, signed(can.size()) - 1)];
                }
            }
            else{
                if (!T::isDead){
                    T::speed = 180 / FPS;
                    if (!T::scatter && !rd(0, 80)) T::scatter = 1;
                    else if (T::scatter && !rd(0, 40)) T::scatter = 0;
                }
                T::getDirection(Pacman::move[tileY - 1][tileX]);
            }
        }
        if (T::direction >= 0){
            T::sprite->destRect->x += cnt * dx[T::direction];
            T::sprite->destRect->y += cnt * dy[T::direction];
        }
    }
    if (T::direction == 0){
        T::sprite->switchAnimation("RunU", frameCount);
    }
    else if (T::direction == 1){
        T::sprite->switchAnimation("RunL", frameCount);
    }
    else if (T::direction == 2){
        T::sprite->switchAnimation("RunD", frameCount);
    }
    else{
        T::sprite->switchAnimation("RunR", frameCount);
    }

    // Check if collide with pacman
    if (Ghost::collide<T>() && !T::isDead){
        if (T::frightened){
            T::frightened = false;
            T::isDead = true;
            T::speed = 540 / FPS;
            T::scatter = false;
            Pacman::score += 200 << Pacman::pelletKill;
            ++Pacman::pelletKill;
            string tem = (Pacman::direction == 0) ? "AttackVUp" : (Pacman::direction == 2) ? "AttackVDown" : "AttackH";
            Pacman::sprite->switchAnimation(tem, frameCount);
            SoundManager::playSound("player.attack");
        }
        else{
            Pacman::lives = -Pacman::lives + 1;
            Pacman::sprite->switchAnimation("Hurt", frameCount);
            SoundManager::playSound("player.hurt");
            Pacman::speed = 0;
        }
    }
}

void Pacman::render(){
    Sprite *spriteCopy = new Sprite(*sprite);
    spriteCopy->destRect = new SDL_Rect();
    spriteCopy->destRect->x = sprite->destRect->x - 28 * 32;
    spriteCopy->destRect->y = sprite->destRect->y;
    spriteCopy->destRect->w = sprite->destRect->w;
    spriteCopy->destRect->h = sprite->destRect->h;
    spriteCopy->render();
    textObj["Score"]->texture = Media::loadTextureFromRenderedText(to_string(Pacman::score), {255, 255, 255}, font, sprite->renderer);
    if (lives && !Pacman::winning){
        textObj["Lives"]->texture = Media::loadTextureFromRenderedText(to_string(Pacman::lives), {255, 255, 255}, font, sprite->renderer);
    }
    else{
        textObj["Lives"]->texture = Media::loadTextureFromRenderedText("Press ESC to exit", {255, 255, 255}, font, sprite->renderer);
        textObj["Lives"]->dstRect->x = 950;
        textObj["Lives"]->dstRect->y = 700;
    }
    for (auto &p: textObj){
        p.second->render(sprite->renderer);
    }
}

template <class T> void Ghost::render(){
    Sprite *spriteCopy = new Sprite(*T::sprite);
    spriteCopy->destRect = new SDL_Rect();
    spriteCopy->destRect->x = T::sprite->destRect->x - 28 * 32;
    spriteCopy->destRect->y = T::sprite->destRect->y;
    spriteCopy->destRect->w = T::sprite->destRect->w;
    spriteCopy->destRect->h = T::sprite->destRect->h;
    spriteCopy->render();
}

template <class T> bool Ghost::collide(){
    int x = Ghost::getX<T>();
    int y = Ghost::getY<T>();
    int x1 = Pacman::getX();
    int y1 = Pacman::getY();
    return abs(x - x1) < 16 && abs(y - y1) < 16;
}

template <class T> void Ghost::reset(){
    
    // Set initial properties of ghost
    T::speed = 180 / FPS;
    T::isDead = false;
    T::spawning = true;
    T::scatter = false;
    T::frightened = false;
    T::sprite->destRect->x = objectGroups["Spawn"]->objectsByName[T::sprite->name][0]->objectX;
    T::sprite->destRect->y = objectGroups["Spawn"]->objectsByName[T::sprite->name][0]->objectY;
    T::sprite->destRect->x = 2 * T::sprite->destRect->x - Ghost::getX<T>();
    T::sprite->destRect->y = 2 * T::sprite->destRect->y - Ghost::getY<T>();
}
#endif