#pragma once
#include <SDL3/SDL.h>

class Sprite {
public:
    Sprite(float x, float y, float w, float h, SDL_Color color);
    virtual ~Sprite();

    virtual void tick();
    virtual void draw(SDL_Renderer* renderer);

    float getX() const {return x;}
    float getY() const {return y;}
    float getWidth() const {return width;}
    float getHeight() const {return height;}
    bool isAlive() const {return alive;}
    SDL_Color getColor() const { return color; }
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setWidth(float newW) { width = newW; }

    void destroy(){alive = false;} // alive = innan för föntstret

    SDL_FRect getRect() const {
        return SDL_FRect{x,y,width,height};
    }

protected:
    float x, y;
    float width, height;
    SDL_Color color;
    bool alive;
};