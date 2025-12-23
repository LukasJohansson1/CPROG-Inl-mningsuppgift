#pragma once
#include <SDL3/SDL.h>

class Sprite {
public:
    Sprite(float x, float y, float w, float h, SDL_Color color);
    virtual ~Sprite();

    virtual void tick();
    virtual void draw(SDL_Renderer* renderer);

protected:
    float x, y;
    float width, height;
    SDL_Color color;
};