// SoundManager.h
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <string>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void playMusic(const std::string& filePath, int loops = -1); // -1 means loop indefinitely
    void stopMusic();

private:
    Mix_Music* music_;
};

#endif
