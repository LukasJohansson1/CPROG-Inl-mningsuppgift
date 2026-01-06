#pragma once
#include "Sprite.h"

class Projectile : public Sprite {
    public:
        Projectile(float x, float y, float speedX, float SpeedY);

        void tick() override;

    private:
        float speedX;
        float speedY;
};