#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

// Sound 클래스: SDL_mixer를 사용하여 사운드를 관리하는 클래스
class Sound {
public:
    // 생성자: SDL_mixer 초기화
    Sound();
    
    // 소멸자: 로드된 소리를 해제하고 SDL_mixer 종료
    ~Sound();

    // 사운드 파일을 로드하여 sounds 맵에 저장
    // path: 사운드 파일 경로
    // soundName: 사운드의 이름 (맵의 키로 사용)
    void loadSound(const std::string& path, const std::string& soundName);
    
    // 사운드를 재생
    // soundName: 재생할 사운드의 이름
    // loops: 반복 재생 횟수 (0이면 한 번, -1이면 무한 반복)
    void playSound(const std::string& soundName, int loops = 0);
    
    // 특정 사운드를 정지
    // soundName: 정지할 사운드의 이름
    void stopSound(const std::string& soundName);
    
    // 모든 사운드를 정지
    void stopAllSounds();
    
    // 특정 사운드를 일시정지
    // soundName: 일시정지할 사운드의 이름
    void pauseSound(const std::string& soundName);
    
    // 일시정지된 사운드를 다시 재생
    // soundName: 재생할 사운드의 이름
    void resumeSound(const std::string& soundName);
    
    // 특정 사운드의 볼륨을 설정
    // soundName: 볼륨을 설정할 사운드의 이름
    // volume: 설정할 볼륨 값 (0 ~ 128)
    void setVolume(const std::string& soundName, int volume);

private:
    // 사운드를 저장하는 맵 (키: 사운드 이름, 값: Mix_Chunk* 포인터)
    std::map<std::string, Mix_Chunk*> sounds;
    
    // 현재 재생 중인 사운드의 이름을 저장하는 벡터
    std::vector<std::string> playingSounds;
};

#endif // SOUND_H
