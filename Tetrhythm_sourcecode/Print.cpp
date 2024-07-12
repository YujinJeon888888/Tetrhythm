#include "Print.h"

Print::Print() {
    SDL_Init(SDL_INIT_VIDEO); // SDL 사용 시작
    window = SDL_CreateWindow("Tetrhythm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 675, 0); // 윈도우를 만든다

    renderer = SDL_CreateRenderer(window, -1, 0); // 렌더러를 만든다
}

Print::~Print() {
    for (auto& layeredTexture : layeredTextures) {
        SDL_DestroyTexture(layeredTexture.texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); // SDL 종료
}

void Print::printPNG(const char* path, const int& dstX, const int& dstY, int layer) {
    SDL_Surface* png = IMG_Load(path);
    if (!png) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, png);
    SDL_FreeSurface(png);

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect dst;
    dst.x = dstX;  // 이미지를 그릴 위치의 왼쪽 상단 x 좌표
    dst.y = dstY;  // 이미지를 그릴 위치의 왼쪽 상단 y 좌표
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h); // 원본 이미지 크기를 그대로 사용

    layeredTextures.push_back({ texture, dst, layer, path });
}

void Print::printAnimationPNG(const std::vector<std::string>& paths, const int& dstX, const int& dstY, int layer, int frameDelay) {
    if (paths.empty()) {
        std::cerr << "No paths provided for animation" << std::endl;
        return;
    }

    Animation anim;
    anim.paths = paths;
    anim.currentFrame = 0;
    anim.frameCount = 0;
    anim.frameDelay = frameDelay;
    anim.layer = layer;

    SDL_Texture* texture = createTextureFromPath(paths[0]);
    if (texture) {
        SDL_QueryTexture(texture, nullptr, nullptr, &anim.animRect.w, &anim.animRect.h);
        anim.animRect.x = dstX;
        anim.animRect.y = dstY;
        layeredTextures.push_back({ texture, anim.animRect, layer, paths[0] });
    }

    animations.push_back(anim);
}

void Print::deletePNG(const char* path) {
    for (size_t i = 0; i < layeredTextures.size(); ++i) {
        if (layeredTextures[i].path == path) {
            SDL_DestroyTexture(layeredTextures[i].texture);
            layeredTextures.erase(layeredTextures.begin() + i);
            return;
        }
    }
    std::cerr << "Failed to delete image: Image not found" << std::endl;
}

void Print::deleteLayer(int layer) {
    for (size_t i = 0; i < layeredTextures.size(); ) {
        if (layeredTextures[i].layer == layer) {
            SDL_DestroyTexture(layeredTextures[i].texture);
            layeredTextures.erase(layeredTextures.begin() + i);
        }
        else {
            ++i;
        }
    }
}

void Print::setLayer(const char* path, int layer) {
    for (auto& layeredTexture : layeredTextures) {
        if (layeredTexture.path == path) {
            layeredTexture.layer = layer;
            return;
        }
    }
    std::cerr << "Failed to set layer: Image not found" << std::endl;
}

SDL_Texture* Print::createTextureFromPath(const std::string& path) {
    SDL_Surface* png = IMG_Load(path.c_str());
    if (!png) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, png);
    SDL_FreeSurface(png);

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return texture;
}

void Print::render() {
    SDL_RenderClear(renderer); // 화면 지우기

    // 레이어 순서에 따라 텍스처 정렬
    std::sort(layeredTextures.begin(), layeredTextures.end(), [](const LayeredTexture& a, const LayeredTexture& b) {
        return a.layer < b.layer;
        });

    for (const auto& layeredTexture : layeredTextures) {
        SDL_RenderCopy(renderer, layeredTexture.texture, nullptr, &layeredTexture.dstRect); // 모든 텍스처를 렌더러에 그린다.
    }

    SDL_RenderPresent(renderer); // 변경된 렌더러를 업데이트
}

void Print::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
    }
}

void Print::updateAnimations() {
    for (auto& anim : animations) {
        anim.frameCount++;
        if (anim.frameCount >= anim.frameDelay) {
            anim.frameCount = 0;
            anim.currentFrame = (anim.currentFrame + 1) % anim.paths.size();

            // 이전 애니메이션 텍스처 삭제
            deleteLayer(anim.layer);

            // 새 텍스처 로드 및 추가
            SDL_Texture* texture = createTextureFromPath(anim.paths[anim.currentFrame]);
            if (texture) {
                layeredTextures.push_back({ texture, anim.animRect, anim.layer, anim.paths[anim.currentFrame] });
            }
        }
    }
}

SDL_Renderer* Print::getRenderer() {

    return renderer;
}