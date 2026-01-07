#pragma once
#include "Sprite.h"

class Ledge : public Sprite {
    public:
        Ledge(float x, float y, float w, float h);

        void tick() override;
};