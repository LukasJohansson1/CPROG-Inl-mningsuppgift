#pragma once
#include "Sprite.h"
#include <vector>
#include <algorithm>

enum class Direction {LEFT, RIGHT};

class Player : public Sprite {
    public:
        Player(float x, float y);

        void tick() override;

        Direction getLastDirection() const {return lastDirection;}

        void setOnGround(bool value) {onGround = value;}
        bool isOnGround() const {return onGround;}
        void setY(float newY) {y = newY;}
        void setX(float newX) {x = newX;}

        float getVelocityY() const {return velocityY;}
        void setVelocityY(float vY) {velocityY = vY;}
        float getVelocityX() const {return velocityX;}
        void setVelocityX(float vX) {velocityX = vX;}

        void takeDamage(float damage) { health = std::max(0.0f, health - damage); }
        void regenerateHealth(float amount) { health = std::min(maxHealth, health + amount); }
        float getHealth() const { return health; }
        void setHealth(float h) { health = std::clamp(h, 0.0f, maxHealth); }

    private:
        Direction lastDirection;
        float velocityX;
        float velocityY;
        bool onGround;

        float health = 100.0f;
        float maxHealth = 100.0f;
};