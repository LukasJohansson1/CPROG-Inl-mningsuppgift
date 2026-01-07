#pragma once
#include "Sprite.h"
#include <vector>

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

    private:
        Direction lastDirection;
        float velocityX;
        float velocityY;
        bool onGround;
};