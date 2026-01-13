#pragma once
#include <memory>
#include <vector>
#include "GameEngine.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Ledge.h"

// Spel klass


class Game {
public:
    Game(int fps = 60);
    ~Game() = default;

    bool init();
    void run();

private:
    std::shared_ptr<GameEngine> engine;
    std::shared_ptr<Player> player;

    // Spawn timers
    Uint64 lastLedgeSpawn = 0;
    Uint64 lastEnemySpawn = 0;

    // Position av senaste plattformen
    float lastLedgeX = 0.0f;
    float lastLedgeY = 0.0f;

    void spawnEnemy();
    void spawnLedge();

    void handlePlayerLedgeCollisions();
    void handlePlayerEnemyCollisions(bool& running);
    void handlePlayerProjectileEnemyCollisions();

    void regeneratePlayerHealth(float amount);
    void updateGameLogic(bool& running);
};
