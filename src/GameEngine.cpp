// Spelmotor

#include <GameEngine.h>
#include <algorithm>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include "Player.h"
#include "Ledge.h"
#include "Projectile.h"
#include "Collision.h"

// Konstruktor
GameEngine::GameEngine(int fps) : fps(fps), gameUpdateCallback(nullptr) {}

// Destruktor
GameEngine::~GameEngine() {
    if(bgtexture) SDL_DestroyTexture(bgtexture);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}

// Initierar SDL och fönster
bool GameEngine::init() {
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Game Engine", constants::gScreenWidth, constants::gScreenHeight, SDL_WindowFlags(0));

    if(!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Surface* bgSurface = IMG_Load(constants::bg_str.c_str());
    if(!bgSurface) {
        std::cerr << "IMG_Load Error: " << SDL_GetError() << std::endl;
        return false;
    }

    bgtexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_DestroySurface(bgSurface);
    if(!bgtexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Lägger till en sprite i spelet
void GameEngine::addSprite(std::shared_ptr<Sprite> sprite) {
    sprites.push_back(sprite);
}

// Tar bort en sprite ur spelet
void GameEngine::removeSprite(std::shared_ptr<Sprite> sprite) {
    sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}

// Hanterar input och events
bool GameEngine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) 
            return false;
        
        // Hantera musinput för projektiler
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
            for (auto& sprite : sprites) {
                if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                    float projSpeed = 10.0f;
                    float speedX = (player->getLastDirection() == Direction::LEFT ? -projSpeed : projSpeed);
                    auto proj = std::make_shared<Projectile>(
                        player->getX() + player->getWidth() / 2 - 5,
                        player->getY() + player->getHeight() / 2 - 5,
                        speedX, 0.0f
                    );
                    addSprite(proj);
                }
            }
        }
    }
    return true;
}

// Uppdaterar kameran och marken
void GameEngine::updateCameraAndGround() {
    // Hitta spelaren och följ med kameran
    for (auto& sprite : sprites) {
        if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
            cameraX = std::max(0.0f, player->getX() - constants::gScreenWidth / 4.0f);
            break;
        }
    }

    // Flytta marken så den alltid täcker skärmen
    for (auto& sprite : sprites) {
        if (auto ledge = std::dynamic_pointer_cast<Ledge>(sprite)) {
            if (ledge->getY() == constants::gScreenHeight - 2.0f) {
                ledge->setX(cameraX - 500.0f);
                ledge->setWidth(constants::gScreenWidth + 1000.0f);
            }
        }
    }
}

// Visar hälsomätare för spelaren
void GameEngine::showHealthBar() {
    for (auto& sprite : sprites) {
        if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
            float health = player->getHealth();
            float healthBarWidth = 100.0f;
            float healthPercent = std::clamp(health / 100.0f, 0.0f, 1.0f);

            // Röd bakgrund
            SDL_FRect healthBarBg{20.0f, 20.0f, healthBarWidth, 20.0f};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &healthBarBg);

            // Grön hälsobar
            SDL_FRect healthBar{20.0f, 20.0f, healthBarWidth * healthPercent, 20.0f};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &healthBar);
            break;
        }
    }
}

// Renderar bakgrund och sprites
void GameEngine::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Rita bakgrund med parallaxeffekt
    float bgWidth = constants::gScreenWidth;
    float bgOffsetX = fmod(cameraX, bgWidth);
    for (int i = -1; i <= 2; i++) {
        SDL_FRect bgRect{i * bgWidth - bgOffsetX, 0, bgWidth, constants::gScreenHeight};
        SDL_RenderTexture(renderer, bgtexture, nullptr, &bgRect);
    }

    // Rita alla sprites
    for (auto& sprite : sprites) {
        SDL_FRect rect = sprite->getRect();
        rect.x -= cameraX;
        SDL_SetRenderDrawColor(renderer, sprite->getColor().r, sprite->getColor().g, sprite->getColor().b, sprite->getColor().a);
        SDL_RenderFillRect(renderer, &rect);
    }
}

// Spelloopen
void GameEngine::run() {
    bool running = true;
    Uint64 ticksPerFrame = 1000 / fps;

    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        if (!handleEvents()) {
            break;
        }

        // Uppdatera alla sprites
        for (auto& sprite : sprites) {
            sprite->tick();
        }

        // Anropa spellogik via callback
        if (gameUpdateCallback) {
            gameUpdateCallback(running);
        }

        if (!running) break;

        // Ta bort döda sprites
        sprites.erase(std::remove_if(sprites.begin(), sprites.end(),
            [](const std::shared_ptr<Sprite>& s){ return !s->isAlive(); }),
            sprites.end());

        // Uppdatera kamera och rendera
        updateCameraAndGround();
        render();
        showHealthBar();
        SDL_RenderPresent(renderer);

        // Frame rate begränsning
        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < ticksPerFrame) {
            SDL_Delay(ticksPerFrame - frameTime);
        }
    }
}