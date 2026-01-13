// SPELKlass

#include "Game.h"
#include "Constants.h"
#include "Collision.h"
#include <algorithm>
#include <iostream>
#include <cmath>

// Konstruktor
Game::Game(int fps) : engine(std::make_shared<GameEngine>(fps)) {}

// Initierar spelet
bool Game::init() {
    if (!engine->init()) {
        return false;
    }
    // Skapa player
    player = std::make_shared<Player>(100, 100);
    engine->addSprite(player);
    // Skapa marken
    auto ground = std::make_shared<Ledge>(0.0f, constants::gScreenHeight - 2.0f, constants::gScreenWidth, 2.0f);
    engine->addSprite(ground);
    // Initiera timers
    lastLedgeSpawn = SDL_GetTicks();
    lastEnemySpawn = SDL_GetTicks();
    lastLedgeX = constants::gScreenWidth - 1100;
    lastLedgeY = constants::gScreenHeight - 2.0f;
    return true;
}

// Skapar en enemy på en viss position
void Game::spawnEnemy() {
    if (!player) return;
    float playerX = player->getX();
    float enemyX = playerX + constants::gScreenWidth - 170.0f;
    float minY = 100.0f;
    float maxY = constants::gScreenHeight - 150.0f;
    float enemyY = minY + static_cast<float>(rand()) / RAND_MAX * (maxY - minY);
    engine->addSprite(std::make_shared<Enemy>(enemyX, enemyY));
}

// Skapar plattformar ledges
void Game::spawnLedge() {
    constexpr float ledgeWidth = 110.0f;
    constexpr float ledgeHeight = 15.0f;
    float x_lower = lastLedgeX + 100.0f + static_cast<float>(rand()) / RAND_MAX * 120.0f;
    float y_lower = constants::gScreenHeight - 150.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;
    // Se till att plattformen inte är för hög eller låg
    engine->addSprite(std::make_shared<Ledge>(x_lower, y_lower, ledgeWidth, ledgeHeight));
    float x_upper = x_lower + 80.0f + static_cast<float>(rand()) / RAND_MAX * 60.0f;
    float y_upper = constants::gScreenHeight / 3.0f + static_cast<float>(rand()) / RAND_MAX * (constants::gScreenHeight / 3.0f);
    engine->addSprite(std::make_shared<Ledge>(x_upper, y_upper, ledgeWidth, ledgeHeight));
    lastLedgeX = x_upper;
    lastLedgeY = y_lower;
    // Begränsa antal plattformar
    std::vector<std::shared_ptr<Ledge>> ledges;
    auto allSprites = engine->getAllSprites();
    for (auto& sprite : allSprites) {
        if (auto l = std::dynamic_pointer_cast<Ledge>(sprite)) {
            if (l->getY() != constants::gScreenHeight - 2.0f)
                ledges.push_back(l);
        }
    }
    if (ledges.size() > 12) {
        engine->removeSprite(ledges.front());
    }
}

// Hanterar kollision mellan spelare och ledges
void Game::handlePlayerLedgeCollisions() {
    if (!player) return;
    player->setOnGround(false);
    SDL_FRect p = player->getRect();
    auto allSprites = engine->getAllSprites();
    for (auto& other : allSprites) {
        if (auto ledge = std::dynamic_pointer_cast<Ledge>(other)) {
            SDL_FRect l = ledge->getRect();
            // Horisontell kollision
            if (intersects(p, l)) {
                if (player->getVelocityX() > 0 && p.x + p.w - player->getVelocityX() <= l.x)
                    player->setX(l.x - p.w);
                else if (player->getVelocityX() < 0 && p.x - player->getVelocityX() >= l.x + l.w)
                    player->setX(l.x + l.w);
            }
            // Vertikal kollision 
            bool horizontalOverlap = p.x + p.w > l.x && p.x < l.x + l.w;
            if (horizontalOverlap && player->getVelocityY() >= 0 && p.y + p.h <= l.y && 
                p.y + p.h + player->getVelocityY() >= l.y) {
                player->setY(l.y - p.h);
                player->setVelocityY(0);
                player->setOnGround(true);
            }
        }
    }
}

// Hanterar kollision mellan player och enemy
void Game::handlePlayerEnemyCollisions(bool& running) {
    if (!player) return;
    SDL_FRect p = player->getRect();
    auto allSprites = engine->getAllSprites();
    for (auto& other : allSprites) {
        if (auto enemy = std::dynamic_pointer_cast<Enemy>(other)) {
            if (intersects(p, enemy->getRect())) {
                player->takeDamage(60);
                player->setHealth(std::clamp(player->getHealth(), 0.0f, 100.0f));
                enemy->destroy();
                // Visa Game Over ruta om spelaren dör
                if (player->getHealth() <= 0.0f) {
                    SDL_ShowSimpleMessageBox( // Vet inte helt om detta ränkas som att använda SDL
                        SDL_MESSAGEBOX_INFORMATION, // men hitta inget bättre sätt att göra en window som är oberoende på platform
                        "Game Over", // du skulle kunna använda windows.h men det fungerar inte på linux/mac
                        "Du dog!",
                        nullptr
                    );
                    running = false;
                    return;
                }
            }
        }
    }
}

// Hanterar kollision mellan projektiler och enemy
void Game::handlePlayerProjectileEnemyCollisions() {
    auto allSprites = engine->getAllSprites();
    for (auto& sprite : allSprites) {
        if (auto proj = std::dynamic_pointer_cast<Projectile>(sprite)) {
            SDL_FRect pRect = proj->getRect();
            for (auto& target : allSprites) {
                if (auto enemy = std::dynamic_pointer_cast<Enemy>(target)) {
                    if (intersects(pRect, enemy->getRect())) {
                        enemy->destroy();
                        proj->destroy();
                    }
                }
            }
        }
    }
}

// Regenererar player health
void Game::regeneratePlayerHealth(float amount) {
    if (player) {
        player->regenerateHealth(amount);
    }
}

// Uppdaterar spellogik varje frame
void Game::updateGameLogic(bool& running) {
    Uint64 now = SDL_GetTicks();
    // Spawnar enemies varannan sekund
    if (now - lastEnemySpawn >= 2000) {
        lastEnemySpawn = now;
        spawnEnemy();
    }
    // Spawnar plattformar var 0.8 sekund
    if (now - lastLedgeSpawn >= 800) {
        lastLedgeSpawn = now;
        spawnLedge();
    }
    // Hantera kollisioner och logik
    handlePlayerProjectileEnemyCollisions();
    handlePlayerEnemyCollisions(running);
    if (!running) return;
    regeneratePlayerHealth(0.02f);
    handlePlayerLedgeCollisions();
}

// Startar spelet
void Game::run() {
    engine->setGameUpdateCallback([this](bool& running) {
        this->updateGameLogic(running);
    });
    engine->run();
}
