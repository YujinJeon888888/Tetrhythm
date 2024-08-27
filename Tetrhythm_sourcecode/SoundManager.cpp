#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    Mix_AllocateChannels(16); // 최대 16개의 채널을 할당하여 다수의 효과음 재생 가능
}

SoundManager::~SoundManager() {
    if (music_) {
        Mix_FreeMusic(music_);
        music_ = nullptr;
    }

    // 모든 로드된 효과음의 메모리는 스마트 포인터에 의해 자동으로 해제됨
    sounds.clear();

    Mix_CloseAudio();
}

void SoundManager::playMusic(const std::string& filePath, int loops) {
    // 이미 음악이 재생 중인지 확인
    if (Mix_PlayingMusic() == 1) {
        return; // 이미 음악이 재생 중이면 함수 종료
    }

    if (music_) {
        Mix_FreeMusic(music_);  // 기존 음악 메모리 해제
        music_ = nullptr;
    }

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

void SoundManager::loadSound(const std::string& path, const std::string& soundName) {
    std::shared_ptr<Mix_Chunk> sound(Mix_LoadWAV(path.c_str()), Mix_FreeChunk);
    if (!sound) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        return;
    }
    sounds[soundName] = sound;
}

void SoundManager::playSound(const std::string& soundName, int loops) {
    try {
        auto it = sounds.find(soundName);

        if (it != sounds.end()) {
            Mix_Chunk* chunk = it->second.get();
            int channel = Mix_PlayChannel(-1, chunk, loops); // -1을 사용하여 빈 채널에서 재생
            if (channel == -1) {
                std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
            }
        }
        else {
            std::cerr << "Sound not found: " << soundName << std::endl;
        }
    }
    catch (std::exception) {

    }
}

void SoundManager::stopSound(const std::string& soundName) {
    auto it = sounds.find(soundName);
    if (it != sounds.end()) {
        Mix_Chunk* chunk = it->second.get();
        for (int i = 0; i < Mix_AllocateChannels(-1); ++i) {
            if (Mix_Playing(i) && Mix_GetChunk(i) == chunk) {
                Mix_HaltChannel(i);
                break;
            }
        }
    }
    else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}
