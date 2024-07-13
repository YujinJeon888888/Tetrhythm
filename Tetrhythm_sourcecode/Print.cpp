#include "Print.h"

Print::Print(WindowManager* wm) : windowManager(wm), renderer(wm->getRenderer()) {}

Print::~Print() {
    for (auto& layeredTexture : layeredTextures) {
        if (layeredTexture.texture != nullptr) {
            SDL_DestroyTexture(layeredTexture.texture);
        }
    }
}

void Print::printPNG(const char* path, const int& dstX, const int& dstY, int layer) {
    SDL_Surface* png = IMG_Load(path);
    if (png == nullptr) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, png);
    SDL_FreeSurface(png);

    if (texture == nullptr) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect dst;
    dst.x = dstX;
    dst.y = dstY;
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);

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

            // 애니메이션 리스트에서도 해당 텍스처를 포함한 애니메이션을 삭제
            for (size_t j = 0; j < animations.size(); ++j) {
                auto it = std::find(animations[j].paths.begin(), animations[j].paths.end(), path);
                if (it != animations[j].paths.end()) {
                    animations.erase(animations.begin() + j);
                    break;
                }
            }

            return;
        }
    }
    std::cerr << "Failed to delete image: Image not found" << std::endl;
}

void Print::deleteAnimation(const std::vector<std::string>& paths) {
    for (size_t i = 0; i < animations.size(); ++i) {
        if (animations[i].paths == paths) {
            int layer = animations[i].layer;
            animations.erase(animations.begin() + i);

            // 해당 레이어의 텍스처들도 삭제
            for (size_t j = 0; j < layeredTextures.size(); ) {
                if (layeredTextures[j].layer == layer) {
                    SDL_DestroyTexture(layeredTextures[j].texture);
                    layeredTextures.erase(layeredTextures.begin() + j);
                }
                else {
                    ++j;
                }
            }

            return;
        }
    }
    std::cerr << "Failed to delete animation: Animation not found" << std::endl;
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

    for (size_t j = 0; j < animations.size(); ) {
        if (animations[j].layer == layer) {
            animations.erase(animations.begin() + j);
        }
        else {
            ++j;
        }
    }
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
    windowManager->clear();

    std::sort(layeredTextures.begin(), layeredTextures.end(), [](const LayeredTexture& a, const LayeredTexture& b) {
        return a.layer < b.layer;
        });

    for (const auto& layeredTexture : layeredTextures) {
        SDL_RenderCopy(renderer, layeredTexture.texture, nullptr, &layeredTexture.dstRect);
    }

    windowManager->present();
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

            SDL_Texture* texture = createTextureFromPath(anim.paths[anim.currentFrame]);
            if (texture) {
                for (auto& layeredTexture : layeredTextures) {
                    if (layeredTexture.layer == anim.layer) {
                        SDL_DestroyTexture(layeredTexture.texture);
                        layeredTexture.texture = texture;
                        layeredTexture.path = anim.paths[anim.currentFrame];
                    }
                }
            }
        }
    }
}

void Print::clearAllPNGs() {
    for (auto& tex : layeredTextures) {
        if (tex.texture != nullptr) {
            SDL_DestroyTexture(tex.texture);
        }
    }
    layeredTextures.clear();
}

void Print::clearAllAnimations() {
    animations.clear();
}

void Print::clearScreen() {
    clearAllAnimations();
    clearAllPNGs();
    windowManager->clear();
    windowManager->present();
}
