#include "Print.h"

Print::Print(WindowManager* wm)
    : windowManager(wm), renderer(wm->getRenderer())
{}

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

            // 占쌍니몌옙占싱쇽옙 占쏙옙占쏙옙트占쏙옙占쏙옙占쏙옙 占쌔댐옙 占쌔쏙옙처占쏙옙 占쏙옙占쏙옙占쏙옙 占쌍니몌옙占싱쇽옙占쏙옙 占쏙옙占쏙옙
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

            // 占쌔댐옙 占쏙옙占싱억옙占쏙옙 占쌔쏙옙처占썽도 占쏙옙占쏙옙
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

void Print::renderForTetris() {
    std::sort(layeredTextures.begin(), layeredTextures.end(), [](const LayeredTexture& a, const LayeredTexture& b) {
        return a.layer < b.layer;
        });

    for (const auto& layeredTexture : layeredTextures) {
        SDL_RenderCopy(renderer, layeredTexture.texture, nullptr, &layeredTexture.dstRect);
    }
}


//텍스트 입력 렌더링도 같이 처리시, 이 메소드 호출
void Print::renderWithTextInputAndPNG() {
    windowManager->clear();

    std::sort(layeredTextures.begin(), layeredTextures.end(), [](const LayeredTexture& a, const LayeredTexture& b) {
        return a.layer < b.layer;
        });

    for (const auto& layeredTexture : layeredTextures) {
        SDL_RenderCopy(renderer, layeredTexture.texture, nullptr, &layeredTexture.dstRect);
    }

    SDL_RenderCopy(renderer, textInputObj.texture, nullptr, &textInputObj.dst);
    //커서깜박임
    static bool cursorVisible = true;
    static Uint32 cursorTimer = SDL_GetTicks();

    // 커서 깜박임 타이머 설정
    if (SDL_GetTicks() - cursorTimer >= 500) { // 500ms마다 깜박임
        cursorVisible = !cursorVisible;
        cursorTimer = SDL_GetTicks();
    }

    // 커서 그리기
    if (cursorVisible) {
        int cursorX = textInputObj.dst.x;
        int cursorHeight = textInputObj.dst.h;  // 커서의 높이를 텍스트 입력란의 높이에 맞춤

        if (!textInput.empty()) {
            cursorX += textInputObj.dst.w;
        }
        SDL_Rect cursorRect = { cursorX, textInputObj.dst.y, 2, cursorHeight };
        SDL_SetRenderDrawColor(renderer, textInputObj.color.r, textInputObj.color.g, textInputObj.color.b, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
    }
    //커서깜박임
    windowManager->present();
}

std::string Print::getTextInput() {
    return this->textInput;
}

std::vector<LayeredTexture> Print::getLayeredTextures() {
    return this->layeredTextures;
}

void Print::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }

    }
}

//첫번째 인자: 엔터누를 시 입력된 텍스트 처리하는 함수 넣기. 
//두 번째 인자: 그 외 처리할 이벤트 넣기
void Print::handleTextEvents(const std::function<void()>& whenSpace, const std::function<void(SDL_Event&)>& onEvent) {
    SDL_Surface* temp = NULL;
    const int maxTextWidth = 400;  // 텍스트가 차지할 수 있는 최대 너비 (입력 상자의 실제 너비에 맞춰 설정)

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }

        switch (event.type)
        {
        case SDL_TEXTINPUT:
        {
            // 새로운 입력을 포함한 텍스트의 전체 너비를 계산
            std::string newTextInput = textInput + event.text.text;
            int textWidth;
            TTF_SizeText(textInputObj.font, newTextInput.c_str(), &textWidth, nullptr);

            // 텍스트 너비가 박스 안의 최대 너비를 초과하지 않을 때만 추가 입력을 허용
            if (textWidth <= maxTextWidth) {
                textInput = newTextInput;

                if (textInputObj.texture) {
                    SDL_DestroyTexture(textInputObj.texture);
                    textInputObj.texture = NULL;
                }

                temp = TTF_RenderText_Solid(textInputObj.font, textInput.c_str(), textInputObj.color);
                if (temp) {
                    textInputObj.texture = SDL_CreateTextureFromSurface(renderer, temp);
                    textInputObj.dst.w = temp->w;
                    textInputObj.dst.h = temp->h;
                    SDL_FreeSurface(temp);
                    temp = NULL;
                }
            }
        }
        break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_BACKSPACE && !textInput.empty()) {
                textInput.pop_back();
                if (textInputObj.texture) {
                    SDL_DestroyTexture(textInputObj.texture);
                    textInputObj.texture = NULL;
                }
                temp = TTF_RenderText_Solid(textInputObj.font, textInput.c_str(), textInputObj.color);
                if (temp) {
                    textInputObj.texture = SDL_CreateTextureFromSurface(renderer, temp);
                    textInputObj.dst.w = temp->w;
                    textInputObj.dst.h = temp->h;
                    SDL_FreeSurface(temp);
                    temp = NULL;
                }
            }
            if ((event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) && !textInput.empty()) {

                whenSpace();

                textInput.clear();
                if (textInputObj.texture) {
                    SDL_DestroyTexture(textInputObj.texture);
                    textInputObj.texture = NULL;
                }
                temp = TTF_RenderText_Solid(textInputObj.font, textInput.c_str(), textInputObj.color);
                if (temp) {
                    textInputObj.texture = SDL_CreateTextureFromSurface(renderer, temp);
                    textInputObj.dst.w = temp->w;
                    textInputObj.dst.h = temp->h;
                    SDL_FreeSurface(temp);
                    temp = NULL;
                }

            }
            onEvent(event); // 특정 조건에서 onEvent 호출

            break;
        }
    }
}

void Print::handleEvents(const std::function<void(SDL_Event&)>& onEvent) {
    //  SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        else {
            onEvent(event); // 특정 조건에서 onEvent 호출
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



void Print::clearScreen() {
    windowManager->clear();
    windowManager->present();
}

void Print::moveImage(const char* path, const int& dstX, const int& dstY) {
    for (auto& layeredTexture : layeredTextures) {
        if (layeredTexture.path == path) {
            layeredTexture.dstRect.x = dstX;
            layeredTexture.dstRect.y = dstY;
            return;
        }
    }
    std::cerr << "Failed to move image: Image not found" << std::endl;
}

SDL_Rect Print::getImagePosition(const char* path) {
    for (const auto& layeredTexture : layeredTextures) {
        if (layeredTexture.path == path) {
            return layeredTexture.dstRect;
        }
    }
    std::cerr << "Failed to get image position: Image not found" << std::endl;
    return SDL_Rect{ 0, 0, 0, 0 }; // 占썩본 占쏙옙환 占쏙옙
}


void Print::printText(const std::string& text, const int& dstX, const int& dstY, int layer, TTF_Font* font, SDL_Color color) { // 폰트출력추가
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect dst;
    dst.x = dstX;
    dst.y = dstY;
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);

    layeredTextures.push_back({ texture, dst, layer, "" });
    FontInfo fontInfo = {
        font,color,layer,dst,texture
    };
    fontInfos.push_back(fontInfo);
}

//input용 텍스트 초기화
void Print::InputText(const int& dstX, const int& dstY, int layer, TTF_Font* font, SDL_Color color)
{
    textInputObj.font = font;
    textInputObj.color = color;
    textInputObj.layer = layer;
    textInputObj.dst.x = dstX;
    textInputObj.dst.y = dstY;
    // 폰트 크기나 텍스트의 높이에 맞추어 동적으로 설정
    int textHeight;
    TTF_SizeText(font, "A", nullptr, &textHeight);  // 단일 문자 'A'의 높이로 텍스트 높이를 측정
    textInputObj.dst.w = 300;  // 너비는 고정된 값으로 설정
    textInputObj.dst.h = textHeight;  // 텍스트 높이로 커서 높이 설정
    textInputObj.texture = NULL;

}

// 폰트출력추가
void Print::setText(int layer, const std::string& newText) {
    for (auto& fontInfo : fontInfos) {
        if (fontInfo.layer == layer) {
            TTF_Font* beforeTextFontInfo = fontInfo.font;

            // 새로운 텍스트를 렌더링하여 SDL_Surface 생성
            SDL_Surface* surface = TTF_RenderText_Blended(fontInfo.font, newText.c_str(), fontInfo.color);
            if (!surface) {
                std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
                return;
            }

            // SDL_Surface에서 SDL_Texture로 변환
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface); // Surface는 이제 필요 없으므로 해제
            if (!texture) {
                std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
                return;
            }

            // 텍스처의 실제 크기를 쿼리하여 dstRect에 설정
            SDL_QueryTexture(texture, nullptr, nullptr, &fontInfo.dst.w, &fontInfo.dst.h);

            // 기존 텍스처를 새로운 텍스처로 대체
            for (auto& layeredTexture : layeredTextures) {
                if (layeredTexture.layer == fontInfo.layer) {
                    SDL_DestroyTexture(layeredTexture.texture); // 이전 텍스처 삭제
                    layeredTexture.texture = texture; // 새 텍스처 설정
                    layeredTexture.dstRect = fontInfo.dst; // 텍스처 크기 설정
                    fontInfo.texture = texture; // 텍스처 정보 갱신
                }
            }
        }
    }
}

//로드폰트: 오버로드( 기울임체)
TTF_Font* Print::loadFont(const char* path, int size, bool italic)
{
    TTF_Font* font = TTF_OpenFont(path, size);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    // 기울임체를 적용할 경우
    if (italic) {
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    }

    return font;
}

void Print::setText(int layer, const std::string& newText, int color) {
    for (auto& fontInfo : fontInfos) {
        if (fontInfo.layer == layer) {
            TTF_Font* beforeTextFontInfo = fontInfo.font;
            if (textAnimations.size() > 0) {
                textAnimations.clear();
            }
            if (color >= 30) {
                //deleteLayer(layer);
                textAnimation(newText, fontInfo.dst.x, fontInfo.dst.y, beforeTextFontInfo, colors, 10, layer);
                // 기존 텍스트를 " " 로
                SDL_Surface* surface = TTF_RenderText_Blended(fontInfo.font, " ", colors[0]);
                if (!surface) {
                    std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
                    return;
                }

                // SDL_Surface에서 SDL_Texture로 변환
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface); // Surface는 이제 필요 없으므로 해제
                if (!texture) {
                    std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
                    return;
                }

                // 텍스처의 실제 크기를 쿼리하여 dstRect에 설정
                SDL_QueryTexture(texture, nullptr, nullptr, &fontInfo.dst.w, &fontInfo.dst.h);

                // 기존 텍스처를 새로운 텍스처로 대체
                for (auto& layeredTexture : layeredTextures) {
                    if (layeredTexture.layer == fontInfo.layer) {
                        SDL_DestroyTexture(layeredTexture.texture); // 이전 텍스처 삭제
                        layeredTexture.texture = texture; // 새 텍스처 설정
                        layeredTexture.dstRect = fontInfo.dst; // 텍스처 크기 설정
                        fontInfo.texture = texture; // 텍스처 정보 갱신
                    }
                }
                return;
            }

            // 새로운 텍스트를 렌더링하여 SDL_Surface 생성
            SDL_Surface* surface = TTF_RenderText_Blended(fontInfo.font, newText.c_str(), colors[color]);
            if (!surface) {
                std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
                return;
            }

            // SDL_Surface에서 SDL_Texture로 변환
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface); // Surface는 이제 필요 없으므로 해제
            if (!texture) {
                std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
                return;
            }

            // 텍스처의 실제 크기를 쿼리하여 dstRect에 설정
            SDL_QueryTexture(texture, nullptr, nullptr, &fontInfo.dst.w, &fontInfo.dst.h);

            // 기존 텍스처를 새로운 텍스처로 대체
            for (auto& layeredTexture : layeredTextures) {
                if (layeredTexture.layer == fontInfo.layer) {
                    SDL_DestroyTexture(layeredTexture.texture); // 이전 텍스처 삭제
                    layeredTexture.texture = texture; // 새 텍스처 설정
                    layeredTexture.dstRect = fontInfo.dst; // 텍스처 크기 설정
                    fontInfo.texture = texture; // 텍스처 정보 갱신
                }
            }
        }
    }
}


TTF_Font* Print::loadFont(const char* path, int size) { // 폰트출력추가
    TTF_Font* font = TTF_OpenFont(path, size);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return font;
}


void Print::unloadFont(TTF_Font* font) { // 폰트출력추가
    if (font) {
        TTF_CloseFont(font);
    }
}
void Print::textAnimation(const std::string& text, const int& dstX, const int& dstY, TTF_Font* font, const std::vector<SDL_Color>& colors, int frameDelay, int layer) {
    TextAnimation anim;
    anim.text = text;
    anim.colors = colors;
    anim.currentFrame = 0;
    anim.frameCount = 0;
    anim.frameDelay = frameDelay;
    anim.layer = layer;
    anim.font = font;

    // 첫 번째 프레임의 텍스트를 렌더링
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), colors[0]);
    if (!surface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect dst;
    dst.x = dstX;
    dst.y = dstY;
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);

    layeredTextures.push_back({ texture, dst, layer, "" });
    anim.textRect = dst;

    textAnimations.push_back(anim);
}


void Print::updateTextAnimation() {
    static std::random_device rd;   // 난수 생성기
    static std::mt19937 gen(rd());  // Mersenne Twister 엔진
    std::uniform_int_distribution<> dis;

    for (auto& anim : textAnimations) {
        anim.frameCount++;
        if (anim.frameCount >= anim.frameDelay) {
            anim.frameCount = 0;

            int newColorIndex;
            do {
                dis = std::uniform_int_distribution<>(0, anim.colors.size() - 1);
                newColorIndex = dis(gen);
            } while (newColorIndex == anim.currentFrame);  // 이전 색상과 동일한 경우 반복

            anim.currentFrame = newColorIndex;

            // 새로운 색상으로 텍스트 렌더링
            SDL_Surface* surface = TTF_RenderText_Blended(anim.font, anim.text.c_str(), anim.colors[anim.currentFrame]);
            if (!surface) {
                std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
                continue;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (!texture) {
                std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
                continue;
            }

            for (auto& layeredTexture : layeredTextures) {
                if (layeredTexture.layer == anim.layer) {
                    SDL_DestroyTexture(layeredTexture.texture);
                    layeredTexture.texture = texture;
                    layeredTexture.dstRect = anim.textRect;
                }
            }
        }
    }
}

