#include "Player.h"
#include <SDL3/SDL.h>
#include "Constants.h"

Player::Player(float x, float y) : Sprite(x,y, 50.0f, 50.0f, SDL_Color(255, 0, 0, 255)), lastDirection(Direction::RIGHT){}

void Player::tick() {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    float speed = 5.0f;

    if(keys[SDL_SCANCODE_W]){ y-= speed;}
    if(keys[SDL_SCANCODE_S]){y+= speed;}
    if(keys[SDL_SCANCODE_A]){x-= speed; lastDirection = Direction::LEFT;}
    if(keys[SDL_SCANCODE_D]){x+= speed; lastDirection = Direction::RIGHT;}



    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + width > constants::gScreenWidth) x = constants::gScreenWidth - width;
    if (y + height > constants::gScreenHeight) y = constants::gScreenHeight - height;
}
