// SPEL Klass

#include "Projectile.h"
#include "Constants.h"

Projectile::Projectile(float x, float y, float speedX, float speedY) : Sprite(x, y, 14.0f , 14.0f, SDL_Color(255, 255, 0, 255)), speedX(speedX), speedY(speedY), spawnTime(SDL_GetTicks()) {}

void Projectile::tick() {
    x+=speedX;
    y+=speedY;

    // Kontrollera om projektilen är utanför skärmen
    // if(x < 0 || x > constants::gScreenWidth * 2 || y < 0 || y > constants::gScreenHeight) {
    //     destroy();
    // }

    // Kontrollera om projektilen har levt för länge
    Uint64 currentTime = SDL_GetTicks();
    if(currentTime - spawnTime > LIFETIME) {
        destroy();
    }
}