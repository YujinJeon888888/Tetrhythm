#include "Sound.h"

Sound::Sound() {
    // 오디오 시스템 초기화
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

// 소멸자: 로드된 소리를 해제하고 SDL_mixer를 종료
Sound::~Sound() {
    // 모든 로드된 소리를 해제
    for (auto& sound : sounds) {
        Mix_FreeChunk(sound.second);
    }
    sounds.clear();
    // 오디오 시스템 종료
    Mix_CloseAudio();
}

// 사운드 파일을 로드하여 sounds 맵에 저장
void Sound::loadSound(const std::string& path, const std::string& soundName) {
    // 파일에서 사운드를 로드
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        return;
    }
    // 사운드를 맵에 저장
    sounds[soundName] = sound;
}

// 사운드를 재생
void Sound::playSound(const std::string& soundName, int loops) {
    // 맵에서 사운드를 찾음
    if (sounds.find(soundName) != sounds.end()) {
        // 사운드를 재생
        int channel = Mix_PlayChannel(-1, sounds[soundName], loops);
        if (channel == -1) {
            std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
        } else {
            playingSounds.push_back(soundName);
        }
    } else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}

// 특정 사운드를 정지
void Sound::stopSound(const std::string& soundName) {
    // 모든 채널을 순회하며 사운드를 찾음
    for (int i = 0; i < Mix_AllocateChannels(-1); ++i) {
        if (Mix_Playing(i) && Mix_GetChunk(i) == sounds[soundName]) {
            Mix_HaltChannel(i);
            break;
        }
    }
}

// 모든 사운드를 정지
void Sound::stopAllSounds() {
    Mix_HaltChannel(-1);
    playingSounds.clear();
}

// 특정 사운드를 일시정지
void Sound::pauseSound(const std::string& soundName) {
    // 모든 채널을 순회하며 사운드를 찾음
    for (int i = 0; i < Mix_AllocateChannels(-1); ++i) {
        if (Mix_Playing(i) && Mix_GetChunk(i) == sounds[soundName]) {
            Mix_Pause(i);
            break;
        }
    }
}

// 일시정지된 사운드를 다시 재생
void Sound::resumeSound(const std::string& soundName) {
    // 모든 채널을 순회하며 사운드를 찾음
    for (int i = 0; i < Mix_AllocateChannels(-1); ++i) {
        if (Mix_Paused(i) && Mix_GetChunk(i) == sounds[soundName]) {
            Mix_Resume(i);
            break;
        }
    }
}

// 특정 사운드의 볼륨을 설정
void Sound::setVolume(const std::string& soundName, int volume) {
    if (sounds.find(soundName) != sounds.end()) {
        Mix_VolumeChunk(sounds[soundName], volume);
    } else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}
