#include "Print.h"

Print::Print() {
    SDL_Init(SDL_INIT_VIDEO); // SDL ��� ����

    window = SDL_CreateWindow("Tetrhythm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, 0); // �����츦 �����
    renderer = SDL_CreateRenderer(window, -1, 0); // �������� �����
}

Print::~Print() {
    for (auto& layeredTexture : layeredTextures) {
        SDL_DestroyTexture(layeredTexture.texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); // SDL ����
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
    dst.x = dstX;  // �̹����� �׸� ��ġ�� ���� ��� x ��ǥ
    dst.y = dstY;  // �̹����� �׸� ��ġ�� ���� ��� y ��ǥ
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h); // ���� �̹��� ũ�⸦ �״�� ���

    layeredTextures.push_back({ texture, dst, layer, path });
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

void Print::setLayer(const char* path, int layer) {
    for (auto& layeredTexture : layeredTextures) {
        if (layeredTexture.path == path) {
            layeredTexture.layer = layer;
            return;
        }
    }
    std::cerr << "Failed to set layer: Image not found" << std::endl;
}

void Print::run(const Uint32& ms) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return;
        }
    }

    SDL_RenderClear(renderer); // ȭ�� �����

    // ���̾� ������ ���� �ؽ�ó ����
    std::sort(layeredTextures.begin(), layeredTextures.end(), [](const LayeredTexture& a, const LayeredTexture& b) {
        return a.layer < b.layer;
        });

    for (const auto& layeredTexture : layeredTextures) {
        SDL_RenderCopy(renderer, layeredTexture.texture, nullptr, &layeredTexture.dstRect); // ��� �ؽ�ó�� �������� �׸���.
    }

    SDL_Delay(ms); // �����ϴ� �ð��� ª������ ������ �����Ӽ��� �ö�
    SDL_RenderPresent(renderer); // ����� �������� ������Ʈ
}

void Print::printAnimationPNG(const std::vector<std::string>& paths, const int& dstX, const int& dstY, int layer, int frameDelay) {
    if (paths.empty()) {
        std::cerr << "No paths provided for animation" << std::endl;
        return;
    }

    int count = 0;
    int i = 0;

    while (true) {
        count++;
        if (count >= frameDelay) {
            count = 0;
            i = (i + 1) % paths.size();

            // ���� �ؽ�ó ����
            for (auto& layeredTexture : layeredTextures) {
                if (layeredTexture.layer == layer) {
                    SDL_DestroyTexture(layeredTexture.texture);
                }
            }
            layeredTextures.erase(std::remove_if(layeredTextures.begin(), layeredTextures.end(),
                [layer](const LayeredTexture& lt) { return lt.layer == layer; }),
                layeredTextures.end());

            // �� �ؽ�ó �ε� �� �߰�
            SDL_Surface* png = IMG_Load(paths[i].c_str());
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
            dst.x = dstX;  // �̹����� �׸� ��ġ�� ���� ��� x ��ǥ
            dst.y = dstY;  // �̹����� �׸� ��ġ�� ���� ��� y ��ǥ
            SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h); // ���� �̹��� ũ�⸦ �״�� ���

            layeredTextures.push_back({ texture, dst, layer, paths[i] });
        }

        run(); // �� ������ ������
    }
}
