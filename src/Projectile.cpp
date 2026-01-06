#include "Projectile.h"
#include "Constants.h"

Projectile::Projectile(float x, float y, float speedX, float speedY) : Sprite(x, y, 8.0f , 8.0f, SDL_Color(255, 255, 0, 255)), speedX(speedX), speedY(speedY){};

void Projectile::tick() {
    x+=speedX;
    y+=speedY;

    if(x < 0 || x > constants::gScreenWidth || y < 0 || y > constants::gScreenHeight) {
        destroy();
    }
}