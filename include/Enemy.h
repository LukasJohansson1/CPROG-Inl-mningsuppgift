#pragma once
#include "Sprite.h"
#include <SDL3/SDL.h>

class Enemy : public Sprite {
private:
    float velocityX;
    Uint64 spawnTime;
    static constexpr Uint64 LIFETIME = 4000; // 3 sekunder

public:
    Enemy(float x, float y)
        : Sprite(x, y, 40.0f, 40.0f, SDL_Color{255, 0, 255, 255}), velocityX(-3.0f), spawnTime(SDL_GetTicks()) {}

    void tick() override {
        x += velocityX;
        
        // Kontrollera lifetime
        Uint64 currentTime = SDL_GetTicks();
        if (currentTime - spawnTime > LIFETIME) {
            destroy();
        }
    }
};
