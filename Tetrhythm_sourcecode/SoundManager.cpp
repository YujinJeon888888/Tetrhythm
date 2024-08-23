#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    if (music_ != nullptr) {
        Mix_FreeMusic(music_);
        Mix_CloseAudio();
    }
}

void SoundManager::playMusic(const std::string& filePath, int loops) {
    music_ = Mix_LoadMUS(filePath.c_str());
    if (!music_) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_PlayMusic(music_, loops);
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}
