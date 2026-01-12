#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "Sprite.h"
#include "Constants.h"
#include "Player.h"
#include "Projectile.h"
#include "Ledge.h"
#include "Enemy.h"


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
    float cameraX = 0.0f;

    // spawn timers
    Uint64 lastLedgeSpawn = 0;
    Uint64 lastEnemySpawn = 0;

    // Position för senaste ledge
    float lastLedgeX = constants::gScreenWidth - 1100;
    float lastLedgeY = constants::gScreenHeight - 2.0f;

    // Spawn-funktioner
    void spawnEnemy();
    void spawnLedge();
};