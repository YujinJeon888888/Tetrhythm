#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <string>
#include <map>
#include <memory>
#include <iostream>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void playMusic(const std::string& filePath, int loops = -1); // 배경음악 재생
    void stopMusic(); // 배경음악 정지

    void loadSound(const std::string& path, const std::string& soundName); // 효과음 로드
    void playSound(const std::string& soundName, int loops = 0); // 효과음 재생
    void stopSound(const std::string& soundName); // 특정 효과음 정지

private:
    Mix_Music* music_ = nullptr;
    std::map<std::string, std::shared_ptr<Mix_Chunk>> sounds; // 스마트 포인터로 효과음 저장
};

#endif // SOUNDMANAGER_H
