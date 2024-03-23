#include "Game.hpp"

Game::Game(const string &title, int xpos, int ypos, int width, int height, bool fullscreen){
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    frameCount = 0;
    keyboard = new Keyboard();
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout << "Subsystem Initialized" << endl;

        //Create new window
        window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, flags);

        if (window){
            cout << "Window created" << endl;
        }

        //Create new renderer
        renderer = SDL_CreateRenderer(window, -1, 0); // -1 is the index of the rendering driver, 0 is the flag
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            cout << "Renderer created" << endl;
        }

        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }

        isRunning = true;
    }
    else{
        // If SDL_Init fails, print the error
        isRunning = false;
        cout << "SDL failed to initialize: " << SDL_GetError() << endl;
    }
}


void Game::preload(){
    // Preload assets here
    ifstream mapData("src/data/map.json");
    map = new Map("Map Scene", renderer, json::parse(mapData));
    map->setCollisionByProperty(new json({{"collision", 1}}));
    // Load characters
    ifstream assets("src/data/assets.json");
    json data = json::parse(assets);
    json images = data["images"];
    for (const json& character : images["characters"]){
        string name = character["name"];
        Sprite *sprite = new Sprite(name, renderer, objectGroups["Spawn"]->objectsByName[name][0]->objectX, objectGroups["Spawn"]->objectsByName[name][0]->objectY, character["w"], character["h"]);
        for (auto &anim : character["sprites"]){
            sprite->addAnimation(anim);
        }
        //Add to sprites manager
        if (sprite->name == "Player")
            sprite->switchAnimation("RunH", 0);
        else
            sprite->switchAnimation("RunD", 0);
        sprites[sprite->name].push_back(sprite);
    }
    for (const json& object: images["objects"]){
        string name = object["name"];
        for (Object* component: objectGroups["Object"]->objectsByName[name]){
            Sprite* sprite = new Sprite(name, renderer, component->objectX, component->objectY, object["w"], object["h"]);
            for (auto &anim: object["sprites"]){
                sprite->addAnimation(anim);
                sprite->switchAnimation("Idle", 0);
            }
            sprites[sprite->name].push_back(sprite);
        }
    }
    // Load Miscellanous
    for (const json& misc: images["misc"]){
        string name = misc["name"];
        Sprite* sprite = new Sprite(name, renderer, 0, 0, misc["w"], misc["h"]);
        for (auto &anim: misc["sprites"]){
            sprite->addAnimation(anim);
            sprite->switchAnimation("Idle", 0);
        }
        if (name == "Target"){
            sprites[sprite->name].push_back(sprite);
            sprites[sprite->name].push_back(new Sprite(*sprite));
            sprites[sprite->name].push_back(new Sprite(*sprite));
            sprites[sprite->name].push_back(new Sprite(*sprite));
            sprites[sprite->name][0]->destRect = sprites["Fox"][0]->destRect;
            sprites[sprite->name][1]->destRect = sprites["Racoon"][0]->destRect;
            sprites[sprite->name][2]->destRect = sprites["Bird"][0]->destRect;
            sprites[sprite->name][3]->destRect = sprites["Cat"][0]->destRect;
        }
    }

    // Load font
    for (const json& font: data["fonts"]){
        string name = font["name"];
        string path = font["path"];
        int size = font["size"];
        fonts[name] = Media::loadFont(path, size);
    }

    json sounds = data["sounds"];
    json music = data["music"];
    // Load sound
    for (const json& character: sounds["characters"]){
        for (const json& sound: character["sounds"]){
            string name = sound["name"];
            string path = sound["path"];
            SoundManager::addSound(name, path);
        }
    }
    for (const json& effect: sounds["effects"]){
        for (const json& sound: effect["sounds"]){
            string name = sound["name"];
            string path = sound["path"];
            SoundManager::addSound(name, path);
        }
    }
    
    // Load music
    for (const json& track: music["bgm"]){
        string name = track["name"];
        string path = track["path"];
        SoundManager::addMusic(name, path);
    }
    Pacman::Init(&sprites, map->layers[1]->tileLayer, map->tileSet, fonts["Monogram Extended"]);
    cout << "Preload Completed" << "\n";
}

void Game::handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
    if (keyboard->isPressed(SDL_SCANCODE_ESCAPE)){
        isRunning = false;
    }
    if (keyboard->isPressed(SDL_SCANCODE_P)){
        PAUSED = !PAUSED;
    }
    string currentAnimation = Pacman::sprite->currentAnimation;
    if (keyboard->isPressed(SDL_SCANCODE_LEFT)){
        if (currentAnimation == "RunH" || currentAnimation == "RunVUp" || currentAnimation == "RunVDown")
            Pacman::sprite->switchAnimation("RunH", frameCount);
        Pacman::sprite->flip = SDL_FLIP_HORIZONTAL;
        Pacman::pendingDirection = 1;
    }
    else if (keyboard->isPressed(SDL_SCANCODE_RIGHT)){
        if (currentAnimation == "RunH" || currentAnimation == "RunVUp" || currentAnimation == "RunVDown")
            Pacman::sprite->switchAnimation("RunH", frameCount);
        Pacman::sprite->flip = SDL_FLIP_NONE;
        Pacman::pendingDirection = 3;
    }
    else if (keyboard->isPressed(SDL_SCANCODE_UP)){
        if (currentAnimation == "RunH" || currentAnimation == "RunVUp" || currentAnimation == "RunVDown")
            Pacman::sprite->switchAnimation("RunVUp", frameCount);
        Pacman::pendingDirection = 0;
    }
    else if (keyboard->isPressed(SDL_SCANCODE_DOWN)){
        if (currentAnimation == "RunH" || currentAnimation == "RunVUp" || currentAnimation == "RunVDown")
            Pacman::sprite->switchAnimation("RunVDown", frameCount);
        Pacman::pendingDirection = 2;
    }
    keyboard->update();
}

void Game::update(){
    //cout << keyboard->isHeld(SDL_SCANCODE_LEFT) << " " << keyboard->isHeld(SDL_SCANCODE_RIGHT) << endl;
    ++frameCount;

    while (!Mix_PlayingMusic()){
        int id = rd(0, signed(SoundManager::musicPath.size()) - 1);
        auto iter = SoundManager::musicPath.begin();
        advance(iter, id);
        SoundManager::playMusic(iter->first);
    }

    SoundManager::update(frameCount);
    
    Pacman::update(frameCount);
    //Do something here
    for (auto &p: sprites){
        for (auto &sprite: p.second){
            sprite->update(frameCount);
        }
    }
    if (Pacman::sprite->anims[Pacman::sprite->currentAnimation]->isPlaying == 0){
        if (Pacman::sprite->currentAnimation == "AttackVUp" || Pacman::sprite->currentAnimation == "AttackVDown" || Pacman::sprite->currentAnimation == "AttackH"){
            if (Pacman::direction == 0)
                Pacman::sprite->switchAnimation("RunVUp", frameCount);
            else if (Pacman::direction == 2)
                Pacman::sprite->switchAnimation("RunVDown", frameCount);
            else
                Pacman::sprite->switchAnimation("RunH", frameCount);
        }
        else if (Pacman::sprite->currentAnimation == "Hurt"){
            Pacman::sprite->switchAnimation("Death", frameCount);
        }
        else if (Pacman::sprite->currentAnimation == "Death" && Pacman::lives){
            Pacman::reset();
        }
    }

}


void Game::render(){
    // Clear the renderer
    SDL_RenderClear(renderer);
    
    // Render stuff here
    map->render();
    for (auto &p: sprites){
        for (auto &sprite: p.second){
            sprite->render();
        }
    }
    Pacman::render();
    Ghost::render<Blinky>();
    Ghost::render<Pinky>();
    Ghost::render<Inky>();
    Ghost::render<Clyde>();
    
    // Add stuff to render here
    RenderManager::render();
    SDL_RenderPresent(renderer);
}

void Game::clean(){
    // Clean the game if it's done
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
    cout << "Game cleaned" << endl;
}

// Deconstructor
Game::~Game(){
    // Clean and destroy the game
    clean();
    cout << "Game destroyed" << endl;
}