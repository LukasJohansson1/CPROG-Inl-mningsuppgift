#include "Sprite.h"



Sprite::Sprite(float x, float y, float w, float h, SDL_Color color)
    : x(x), y(y), width(w), height(h), color(color), alive(true) {}

Sprite::~Sprite() {}

void Sprite::tick() {
}

void Sprite::draw(SDL_Renderer* renderer) {
    SDL_FRect rect{x, y, width, height};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

