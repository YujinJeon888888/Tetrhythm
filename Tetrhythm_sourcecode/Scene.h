#pragma once

#include "Print.h"

class Scene {
public:
    virtual ~Scene() = default;
    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void drawInit() = 0;
};