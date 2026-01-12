#pragma once
#include "Sprite.h"

class Projectile : public Sprite {
public:
    Projectile(float x, float y, float speedX, float speedY);
    void tick() override;

private:
    float speedX, speedY;
    Uint64 spawnTime;
    static constexpr Uint64 LIFETIME = 3200; // 3.2 sekunder i millisekunder
};