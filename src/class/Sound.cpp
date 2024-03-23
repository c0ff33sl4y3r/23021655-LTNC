#ifndef Sound_cpp
#define Sound_cpp

#include "Sound.hpp"

SoundManager::SoundManager(){
}

SoundManager::~SoundManager(){
    clear();
}

void SoundManager::playSound(string name, int loop = 0){
    if(soundPath.find(name) == soundPath.end()){
        cout << "Sound " << name << " not found" << endl;
        return;
    }
    sound[name].push_back(Media::loadSound(soundPath[name]));
    if (sound[name].back() == nullptr){
        cerr << "Failed to play sound: " << name << endl;
        sound[name].pop_back();
        if (sound[name].empty()){
            sound.erase(name);
        }
        return;
    }
    int channel = Mix_PlayChannel(-1, sound[name].back(), loop);
    channels[name].push_back(channel);
}

void SoundManager::playMusic(string name, int loop = 0){
    if(musicPath.find(name) == musicPath.end()){
        cout << "Music " << name << " not found" << endl;
        return;
    }
    music[name].push_back(Media::loadMusic(musicPath[name]));
    if (music[name].back() == nullptr){
        cerr << "Failed to play music: " << name << endl;
        music[name].pop_back();
        if (music[name].empty()){
            music.erase(name);
        }
        return;
    }
    Mix_PlayMusic(music[name].back(), loop);
}

void SoundManager::stopMusic(string name){
    if (music.find(name) != music.end()){
        Mix_HaltMusic();
        Mix_FreeMusic(music[name].front());
        music[name].pop_front();
        if (music[name].empty()){
            music.erase(name);
        }
    }
}

void SoundManager::stopSound(string name){
    if (sound.find(name) != sound.end()){
        Mix_HaltChannel(channels[name].front());
        Mix_FreeChunk(sound[name].front());
        sound[name].pop_front();
        channels[name].pop_front();
        if (sound[name].empty()){
            sound.erase(name);
        }
    }
}

void SoundManager::pauseMusic(string name){
    if (music.find(name) != music.end()){
        Mix_PauseMusic();
    }
}

void SoundManager::pauseSound(string name){
    if (channels.find(name) != channels.end()){
        for (auto &channel : channels[name]){
            Mix_Pause(channel);
        }
    }
}

void SoundManager::resumeMusic(string name){
    if (music.find(name) != music.end()){
        Mix_ResumeMusic();
    }
}

void SoundManager::resumeSound(string name){
    if (channels.find(name) != channels.end()){
        for (auto &channel : channels[name]){
            Mix_Resume(channel);
        }
    }
}

void SoundManager::setVolume(string name, int volume){
    if (channels.find(name) != channels.end()){
        for (auto &channel : channels[name]){
            Mix_Volume(channel, volume);
        }
    }
}

void SoundManager::addSound(string name, string file){
    soundPath[name] = file;
}

void SoundManager::addMusic(string name, string file){
    musicPath[name] = file;
}

void SoundManager::removeSound(string name){
    if (sound.find(name) != sound.end()){
        for (auto &s : sound[name]){
            Mix_FreeChunk(s);
        }
        sound.erase(name);
        channels.erase(name);
    }
}

void SoundManager::removeMusic(string name){
    if (music.find(name) != music.end()){
        for (auto &m : music[name]){
            Mix_FreeMusic(m);
        }
        music.erase(name);
    }
}

bool SoundManager::isPlayingSound(string name){
    return sound.find(name) != sound.end();
}

bool SoundManager::isPlayingMusic(string name){
    return music.find(name) != music.end();
}

void SoundManager::clear(){
    for (auto &s : sound){
        for (auto &ss : s.second){
            Mix_FreeChunk(ss);
        }
    }
    for (auto &m : music){
        for (auto &mm : m.second){
            Mix_FreeMusic(mm);
        }    
    }
    sound.clear();
    music.clear();
    soundPath.clear();
    musicPath.clear();
    channels.clear();
}
void SoundManager::update(int frameCount){
    expires.clear();
    for (auto it = sound.begin(); it != sound.end();){
        for (int i = signed(it->second.size()) - 1; i >= 0; i--){
            if (Mix_Playing(channels[it->first][i]) == 0){
                swap(it->second[i], it->second.back());
                swap(channels[it->first][i], channels[it->first].back());
                expires.push_back(it->first);
                Mix_FreeChunk(it->second.back());
                it->second.pop_back();
                channels[it->first].pop_back();
            }
        }
        if (it->second.empty()){
            channels.erase(it->first);
            it = sound.erase(it);
        }
        else {
            ++it;
        }
    }
    for (auto it = music.begin(); it != music.end();){
        for (int i = signed(it->second.size()) - 1; i >= 0; i--){
            if (Mix_PlayingMusic() == 0){
                swap(it->second[i], it->second.back());
                expires.push_back(it->first);
                Mix_FreeMusic(it->second.back());
                it->second.pop_back();
            }
        }
        if (it->second.empty()){
            it = music.erase(it);
        }
        else {
            ++it;
        }
    }
}

unordered_map <string, string> SoundManager::soundPath;
unordered_map <string, string> SoundManager::musicPath;
unordered_map <string, deque <int>> SoundManager::channels;
unordered_map <string, deque <Mix_Music*>> SoundManager::music;
unordered_map <string, deque <Mix_Chunk*>> SoundManager::sound;
vector <string> SoundManager::expires;

#endif