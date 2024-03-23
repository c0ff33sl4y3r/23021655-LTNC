#ifndef Sound_hpp
#define Sound_hpp
#include "Config.cpp"
#include "Media.cpp"

struct SoundManager{
    SoundManager();
    ~SoundManager();
    static void playSound(string name, int loop);
    static void playMusic(string name, int loop);
    static void stopMusic(string name);
    static void stopSound(string name);
    static void pauseMusic(string name);
    static void pauseSound(string name);
    static void resumeMusic(string name);
    static void resumeSound(string name);
    static void setVolume(string name, int volume);
    static void addSound(string name, string file);
    static void addMusic(string name, string file);
    static void removeSound(string name);
    static void removeMusic(string name);
    static bool isPlayingSound(string name);
    static bool isPlayingMusic(string name);
    
    static void clear();
    /**
     * @brief This function should be called every frame
     * to prevent memory leak
     * 
     * @param frameCount
     */
    static void update(int frameCount);
    /**
     * @brief Store the path of the sound and will be call to prevent memory leak
     * 
     */
    static unordered_map <string, string> soundPath;
    static unordered_map <string, string> musicPath;
    static unordered_map <string, deque <int>> channels;
    static unordered_map <string, deque <Mix_Music*>> music;
    static unordered_map <string, deque <Mix_Chunk*>> sound;
    static vector <string> expires;
};

#endif