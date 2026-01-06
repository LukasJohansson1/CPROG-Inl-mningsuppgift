#pragma once
#include "Sprite.h"

enum class Direction {LEFT, RIGHT};

class Player : public Sprite {
    public:
        Player(float x, float y);

        void tick() override;

        Direction getLastDirection() const {return lastDirection;}

    private:
        Direction lastDirection;
};