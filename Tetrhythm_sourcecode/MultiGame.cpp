#include "Multi.h"
#include "MultiGame.h"
#include <stdexcept>
#include <iostream>
#include "tetromino.h"
#include "Well.h"
#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <cmath>  // std::round를 사용하기 위해 필요
#include <thread>
#include <chrono>
#include "Windows.h"
#include <functional>


const std::string MultiHeart::paths[3] = {
    "heart1.png",
    "heart2.png",
    "heart3.png"
};
const int MultiHeart::xPositions[3] = {
    857,
    910,
    963
};
const int MultiHeart::yPositions[3] = {
    50,
    50,
    50
};

const int MultiHeart::xPositions_opponent[3] = {
    118,
    171,
    224
};


MultiGame::MultiGame(WindowManager& wm, Print* pr, SceneManager& sm)
    :
    sceneManager(sm),
    well_(801, 100, 1081, 165), // 플레이어 보드
    tetromino_(801, 100), // 플레이어 보드
    nextTetrominos_{ Tetromino{801, 100}, Tetromino{801, 100}, Tetromino{801, 100} }, // 다음 블럭 3개 초기화
    opponentWell_(61, 100,344,165), // 상대방 보드
    moveTime_(SDL_GetTicks() + 1000),
    previousLine(0),
    previousTetris(0),
    score(0),
    gameOver(false),
    windowManager(wm),
    print(pr),
    heartPosX(60),
    heartSpeed(5),
    heartVisible(false),
    timeSinceStart(0.0),
    lastFrameTime(std::chrono::steady_clock::now()),
    musicPlayed(false),
    soundManager(new SoundManager()),
    heartSpawnInterval(60.0 / 140.0 * 4),
    nextHeartSpawnTime(0.0),
    countdown3Displayed(false),
    countdown2Displayed(false),
    countdown1Displayed(false),
    startDisplayed(false),
    startDeleted(false),
    perfectImageStartTime(0.0), // 초기화
    perfectImageVisible(false)
{
    
    opponentWell_.isOpponent = true;
    const char* textureFiles[7] = {
        "Skyblue_I.png",
        "Blue_J.png",
        "Orange_L.png",
        "Yellow_O.png",
        "Green_S.png",
        "Purple_T.png",
        "Red_Z.png"
    };


    for (int i = 0; i < 7; ++i)
    {
        SDL_Surface* surface = IMG_Load(textureFiles[i]);
        if (!surface)
        {
            throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
        }
        SDL_Renderer* renderer = windowManager.getRenderer();
        if (!renderer) {
            throw std::runtime_error("Renderer is not initialized properly.");
        }

       // std::cout << i << blockTextures_[i];
        blockTextures_[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!blockTextures_[i])
        {
            throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
        }
    }

    SDL_Surface* surface = IMG_Load("Grayblock.png");
    if (!surface)
    {
        throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
    }
    grayBlockTexture_ = SDL_CreateTextureFromSurface(windowManager.getRenderer(), surface);
    SDL_FreeSurface(surface);
    if (!grayBlockTexture_)
    {
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
    }


}


MultiGame::~MultiGame()
{
    soundManager->stopMusic();
    delete soundManager;
    soundManager = nullptr;

    for (int i = 0; i < 7; ++i)
    {
        if (blockTextures_[i])
        {
            SDL_DestroyTexture(blockTextures_[i]);
            blockTextures_[i] = nullptr;
        }
    }
}


bool MultiGame::tick()
{
    static Uint32 lastMoveTime = 0;
    static Uint32 lastRotationTime = 0;
    static Uint32 lastDropTime = 0;
    static double lastLogTime = 0.0;  // 경과 시간을 출력한 마지막 시간 기록

    Uint32 currentTime = SDL_GetTicks();
    Uint32 moveDelay = 50;   // 이동 시 딜레이 (밀리초 단위)
    Uint32 rotateDelay = 50; // 회전 시 딜레이 (밀리초 단위)
    Uint32 dropDelay = 50;   // 드랍 시 딜레이 (밀리초 단위)

    if (hearts.size() == 0)
    {
        gameOver = true;
      //  std::cout << "Game Over!" << std::endl;
    }

    int type = Multi::getInstance()->receiveMessegeData();

    if (type == 2|| Multi::getInstance()->isClear) //클리어 임시 구?현
    {
         Multi::resetInstance();
        soundManager->stopMusic(); // TetrisScene 객체가 파괴될 때 음악을 중지
        delete soundManager;
        isClear = true;
        gameOver = true;
       // return false;
    }

    if (type == 4) {
        std::cout << "-----";
        deductHeart_opponent();
    }
    else if(type == 5) {
    
    }

    if (gameOver) {
        Multi::getInstance()->sendGameOver();
      //  Multi::getInstance()->isClear = true;
        soundManager->stopMusic(); // 다른 창으로 이동하기 전에 음악을 중지합니다.
        //최대콤보반영
        comboVector.push_back(comboCount);
        std::sort(comboVector.begin(), comboVector.end(), std::greater<int>());//내림차순정렬
        score += (int)(std::round((float)comboScore * ((float)comboVector[0] / (float)fullComboCount)));
        return false;
    }


    SDL_SetRenderDrawColor(windowManager.getRenderer(), 0, 0, 0, 0xff);
    SDL_RenderClear(windowManager.getRenderer());
    print->renderForTetris();

    // 그림자 위치 계산 및 그리기
    Tetromino shadow = tetromino_.calculateShadow(well_);
    well_.drawShadow(windowManager.getRenderer(), blockTextures_[tetromino_.getType()], shadow);
    well_.draw(windowManager.getRenderer(), blockTextures_, grayBlockTexture_, nextTetrominos_);
    tetromino_.draw(windowManager.getRenderer(), blockTextures_[tetromino_.getType()]);

    //상대방 보드(+대기열) 그리기
    opponentWell_.draw(windowManager.getRenderer(), blockTextures_, grayBlockTexture_, nextTetrominos_);
    // 상대 필드를 빈 상태로 그리기


    SDL_Event e;


    if (timeSinceStart >= 5.0 && !musicPlayed)
    {
        soundManager->playMusic("Musics/Megalovania 8Bit Remix Audio.mp3", -1);
        musicPlayed = true;
    }

   // 프레임 타이밍 관리
    auto currentFrameTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    timeSinceStart += deltaTime.count();

    //perfect이미지 삭제되게하기 
    if (perfectImageVisible && (timeSinceStart - perfectImageStartTime) >= 0.5) {
        print->deletePNG("Perfect.png");
        perfectImageVisible = false;
    }

    //// 카운트다운 이미지 출력 및 삭제
    if (timeSinceStart >= 0.0 && timeSinceStart < 1.0 && !countdown3Displayed) {
        soundManager->playMusic("Musics/Countdown.mp3", 1); // 배경음악 재생
        print->printPNG("Countdown3.png", 604, 271, 10);
        countdown3Displayed = true;
    }
    if (timeSinceStart >= 1.0 && timeSinceStart < 2.0 && !countdown2Displayed) {
        print->deletePNG("Countdown3.png");
        print->printPNG("Countdown2.png", 611, 274, 11);
        countdown2Displayed = true;
    }
    if (timeSinceStart >= 2.0 && timeSinceStart < 3.0 && !countdown1Displayed) {
        print->deletePNG("Countdown2.png");
        print->printPNG("Countdown1.png", 605, 274, 12);
        countdown1Displayed = true;
    }
    if (timeSinceStart >= 3.0 && timeSinceStart < 4.0 && !startDisplayed) {
        print->deletePNG("Countdown1.png");
        print->printPNG("Start.png", 522, 274, 13);
        startDisplayed = true;
    }
    if (timeSinceStart >= 4.0 && !startDeleted) {
        print->deletePNG("Start.png");
        startDeleted = true; // 삭제되었음을 표시
    }

    // 이벤트 처리
    if (timeSinceStart >= 4.0) {
        if (SDL_WaitEventTimeout(&e, 10))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_SPACE:
                    std::cout << "space " << forDebugCount++ << std::endl;
                    if (currentTime > lastDropTime + dropDelay)
                    {
                        Tetromino t = tetromino_;
                        t.drop(well_);
                        check(t);
                        lastDropTime = currentTime; // 마지막 드랍 시간 기록

                        // 하트 노트 위치 판정
                        if (heartVisible)
                            soundManager->playSound("BlockDrop", 0);
                        {
                            if (690 <= heartPosX && heartPosX <= 768)
                            {
                                comboCount += 1;
                                score += (heartPosX == 729) ? 2000 : 500;
                                print->setText(9, "       " + std::to_string(score));
                                std::cout << "safe!" << std::endl;
                                heartVisible = false;
                                print->deletePNG("heartNote.png");
                                if (heartPosX == 729){
                                    std::cout << "perfect!" << std::endl;
                                    print->printPNG("Perfect.png", 376, 169, 1);
                                    perfectImageStartTime = timeSinceStart; // 표시 시점 기록
                                    perfectImageVisible = true;
                                }
                            }
                            else if (heartPosX <= 690)
                            {
                                // 이 부분에서 하트 노드를 바로 사라지게 처리
                                if (!heartDeduct) {
                                    deductHeart();
                                }
                                heartDeduct = true;
                                heartVisible = false;
                                print->deletePNG("heartNote.png");
                            }

                            // 다음 하트 노드 생성 타이밍 설정
                            nextHeartSpawnTime = timeSinceStart + heartSpawnInterval;
                        }
                    }

                    break;
                case SDLK_ESCAPE:
                    soundManager->stopMusic();
                    soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
                    soundManager->playSound("Selection", 0);
                    sceneManager.changeScene(std::make_unique<MainMenu>(windowManager, sceneManager));
                    break;
                case SDLK_DOWN:
                    if (currentTime > lastMoveTime + moveDelay)
                    {
                        Tetromino t = tetromino_;
                        t.move(0, 1);
                        if (!well_.isCollision(t)) {
                            tetromino_ = t;
                        }
                        else {
                            soundManager->playSound("BlockDrop", 0);
                            // 하트 노트 위치 판정
                            if (heartVisible)
                            {
                                if (393 < heartPosX && heartPosX < 471)
                                {
                                    comboCount += 1;
                                    score += (heartPosX == 432) ? 2000 : 500;
                                    print->setText(9, "       " + std::to_string(score));
                                    //std::cout << "safe!" << std::endl;
                                    heartVisible = false;
                                    print->deletePNG("heartNote.png");
                                    if (heartPosX == 432)
                                    {
                                        std::cout << "perfect!" << std::endl;
                                        print->printPNG("Perfect.png", 376, 169, 1);
                                        perfectImageStartTime = timeSinceStart; // 표시 시점 기록
                                        perfectImageVisible = true;
                                    }
                                }
                                else if (heartPosX <= 393)
                                {
                                    // 이 부분에서 하트 노드를 바로 사라지게 처리
                                    if (!heartDeduct) {
                                        deductHeart();
                                    }
                                    heartDeduct = true;
                                    heartVisible = false;
                                    print->deletePNG("heartNote.png");
                                }

                                // 다음 하트 노드 생성 타이밍 설정
                                nextHeartSpawnTime = timeSinceStart + heartSpawnInterval;
                            }

                        }
                        moveTime_ = SDL_GetTicks() + 400; // 아래로 이동 시 자동 내려오는 시간을 조정
                        lastMoveTime = currentTime; // 마지막 이동 시간 기록
                    }
                    break;

                case SDLK_RIGHT:
                case SDLK_LEFT:
                    if (currentTime > lastMoveTime + moveDelay)
                    {
                        Tetromino t = tetromino_;
                        t.move((e.key.keysym.sym == SDLK_RIGHT) ? 1 : -1, 0);
                        if (!well_.isCollision(t)) {
                            tetromino_ = t;
                        }

                        lastMoveTime = currentTime; // 마지막 이동 시간 기록
                    }
                    break;

                case SDLK_a:
                case SDLK_z:
                    if (currentTime > lastRotationTime + rotateDelay)
                    {
                        Tetromino t = tetromino_;
                        t.rotateCounterClockwise();
                        if (!well_.isCollision(t)) {
                            tetromino_ = t;
                        }

                        lastRotationTime = currentTime; // 마지막 회전 시간 기록
                    }
                    break;

                case SDLK_d:
                case SDLK_c:
                case SDLK_UP:
                    if (currentTime > lastRotationTime + rotateDelay)
                    {
                        Tetromino t = tetromino_;
                        t.rotate();
                        if (!well_.isCollision(t)) {
                            tetromino_ = t;
                        }

                        lastRotationTime = currentTime; // 마지막 회전 시간 기록
                    }
                    break;
                }
                break;


            case SDL_QUIT:
                exit(0);
                return false;
            }
        }
        // 자동으로 블럭이 내려오도록 설정
        if (SDL_GetTicks() > moveTime_)
        {
            Tetromino t = tetromino_;
            Tetromino temp = tetromino_;
            t.move(0, 1);
            //temp는 move보다 한 칸 더 감
            temp.move(0, 1);
            temp.move(0, 1);
            if (!well_.isCollision(t))
            {
                tetromino_ = t;
                moveTime_ = SDL_GetTicks() + 1000; // 다음 내려오는 시간 설정         
            }
            else//충돌
            {

                check(t);
            }

            if (well_.isCollision(temp) && !well_.isCollision(t)) {
                // 하트 노트 위치 판정
                if (heartVisible)
                {
                    if (393 < heartPosX && heartPosX < 471)
                    {
                        comboCount += 1;
                        score += (heartPosX == 432) ? 2000 : 500;
                        print->setText(9, "       " + std::to_string(score));
                        //std::cout << "safe!" << std::endl;
                        heartVisible = false;
                        print->deletePNG("heartNote.png");
                        if (heartPosX == 432) {
                            std::cout << "perfect!" << std::endl;
                            print->printPNG("Perfect.png", 376, 169, 1);
                            perfectImageStartTime = timeSinceStart; // 표시 시점 기록
                            perfectImageVisible = true;
                        }
                    }
                    else if (heartPosX <= 393)
                    {
                        // 이 부분에서 하트 노드를 바로 사라지게 처리
                        if (!heartDeduct) {
                            deductHeart();
                        }
                        heartDeduct = true;
                        heartVisible = false;
                        print->deletePNG("heartNote.png");
                    }

                    // 다음 하트 노드 생성 타이밍 설정
                    nextHeartSpawnTime = timeSinceStart + heartSpawnInterval;
                }

                std::cout << "reach!" << std::endl;
            }
        }

        int currentLine = well_.getLine();
        int currentTetris = well_.getTetris();

        if (currentLine > previousLine)
        {
            soundManager->playSound("LineClear", 0);
            std::cout << "Line: " << currentLine << std::endl;
            int linesCleared = currentLine - previousLine;
            previousLine = currentLine;
            print->setText(7, "      " + std::to_string(previousLine));

            switch (linesCleared) { // 1, 2, 3줄에 대한 점수 계산
            case 1:
                score += 900;
                break;
            case 2:
                score += 1700;
                break;
            case 3:
                score += 2500;
                break;
            default:
                break;
            }


            print->setText(9, "       " + std::to_string(score));
        }

        //테트리스
        if (currentTetris > previousTetris)
        {
            // 4줄 깬 경우
            seriesTetrisCount += 1;
            if (seriesTetrisCount % 2 == 0) {
                score += 10000;  // 연속 테트리스
            }
            else {
                score += 4000;  // 단일 테트리스
            }
            //하트 맥시멈(3)보다 작을때만, 목숨 추가.
            if (hearts.size() < Heart::maxHeart && hearts.size() != 0) {
                MultiHeart heart{ MultiHeart::paths[hearts.size()], MultiHeart::xPositions[hearts.size()], MultiHeart::yPositions[hearts.size()] };
                hearts.push_back(heart);
                print->printPNG(heart.path.c_str(), heart.xPosition, heart.yPosition);
            }

            std::cout << "Tetris: " << currentTetris << std::endl;
            previousTetris = currentTetris;
            print->setText(8, "        " + std::to_string(previousTetris));

        }
        else {
            seriesTetrisCount = 0;
        }




        if (timeSinceStart >= 224.0) //클리어 임시 구?현
        {
            soundManager->playSound("YouWin", 0);
            soundManager->stopMusic(); // TetrisScene 객체가 파괴될 때 음악을 중지
            delete soundManager;
            isClear = true;
            gameOver = true;

        }

        if (hearts.size() == 0)
        {
            gameOver = true;
            std::cout << "Game Over!" << std::endl;
        }

        if (gameOver) {
            soundManager->stopMusic(); // 다른 창으로 이동하기 전에 음악을 중지합니다.
            //최대콤보반영
            comboVector.push_back(comboCount);
            std::sort(comboVector.begin(), comboVector.end(), std::greater<int>());//내림차순정렬
            score += (int)(std::round((float)comboScore * ((float)comboVector[0] / (float)fullComboCount)));
            return false;
        }

        if (heartVisible)
        {
            SDL_Rect rect = print->getImagePosition("heartNote.png");
            SDL_Point currentPosition = { rect.x, rect.y };
            double totalDuration = 4 * beatInterval;
            double moveDistance = (768 - 462) * (deltaTime.count() / totalDuration);
            int newPosX = currentPosition.x + moveDistance;

            heartPosX = newPosX;
            print->moveImage("heartNote.png", heartPosX, currentPosition.y);

            if (heartPosX > 768)
            {
                if (!heartDeduct) {
                    deductHeart();
                }
                heartDeduct = true;
                heartVisible = false;
                print->deletePNG("heartNote.png");
                // 다음 하트 노드 생성 타이밍 설정
                nextHeartSpawnTime = timeSinceStart + heartSpawnInterval;
            }
        }
        else if (timeSinceStart >= nextHeartSpawnTime && !heartVisible)
        {
            // 다음 하트 노드를 생성할 시간에 도달했을 때만 생성
            heartVisible = true;
            heartPosX = 462;
            print->printPNG("heartNote.png", heartPosX, 225, 11);
        }


    }
    SDL_RenderPresent(windowManager.getRenderer());

    heartDeduct = false;
    return true;
}


void MultiGame::check(const Tetromino& t)
{
    if (t.y() >= 0 && well_.isCollision(t))
    {
        well_.unite(tetromino_);  // 블록을 well에 추가

        // 블록이 추가된 후 새로운 블록을 생성합니다.
        tetromino_ = nextTetrominos_[0];
        nextTetrominos_[0] = nextTetrominos_[1];
        nextTetrominos_[1] = nextTetrominos_[2];
        nextTetrominos_[2] = Tetromino(801, 100);  // 새로운 위치로 테트로미노를 생성

        // 새로운 블록이 Well에 충돌하면 게임 오버 처리
        if (well_.isCollision(tetromino_))
        {
            gameOver = true;
           // std::cout << "Game Over!" << std::endl;
        }
    }
    else  // 충돌이 발생하지 않았을 경우에만 tetromino 업데이트
    {
        tetromino_ = t;
    }
}



bool MultiGame::isGameOver() const
{
    return gameOver;
}

SDL_Texture* MultiGame::getBlockTexture(Tetromino::Type type) const
{
    return blockTextures_[type];
}

void MultiGame::deductHeart()
{
    isPerfectClear = false;
    comboVector.push_back(comboCount);
    comboCount = 0;
 //   std::cout << "when heartPosX : " << heartPosX << "deduct heart" << std::endl;
    if (!hearts.empty())
    {
        Multi::getInstance()->sendHeartInfo("minus");
        MultiHeart lastHeart = hearts.back();
        print->deletePNG(lastHeart.path.c_str());
        hearts.pop_back();
     //   std::cout << "Heart deducted! Remaining hearts: " << hearts.size() << std::endl;
    }
    else
    {
      //  std::cout << "No hearts left to deduct" << std::endl;
    }
}

void MultiGame::deductHeart_opponent()
{
  
    //   std::cout << "when heartPosX : " << heartPosX << "deduct heart" << std::endl;
    if (!oppnentHearts.empty())
    {
      //  Multi::getInstance()->sendHeartInfo("minus");
        MultiHeart lastHeart = oppnentHearts.back();
        print->deletePNG(lastHeart.path.c_str());
        oppnentHearts.pop_back();
        //   std::cout << "Heart deducted! Remaining hearts: " << hearts.size() << std::endl;
    }
    else
    {
        //  std::cout << "No hearts left to deduct" << std::endl;
    }
}