#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "Sprite.h"
#include "Constants.h"
#include "Player.h"
#include "Projectile.h"
#include "Ledge.h"


class GameEngine {

public:
    GameEngine(int fps = 60);
    ~GameEngine();

    bool init();
    void run();

    void addSprite(std::shared_ptr<Sprite> sprite);
    void removeSprite(std::shared_ptr<Sprite> sprite);

private:
    int fps;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* bgtexture = nullptr;
    std::vector<std::shared_ptr<Sprite>> sprites;
};