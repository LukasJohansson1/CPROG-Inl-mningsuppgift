#include "Player.h"
#include <SDL3/SDL.h>
#include "Constants.h"

Player::Player(float x, float y) : Sprite(x,y, 50.0f, 50.0f, SDL_Color(255, 0, 0, 255)), lastDirection(Direction::RIGHT), velocityX(0.0f), velocityY(0.0f), onGround(false){}

void Player::tick() {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    float moveSpeed = 5.5f;
    float gravity = 0.5f;
    float jumpSpeed = -16.0f;

    velocityX = 0.0f;

    if(keys[SDL_SCANCODE_W] && onGround){ velocityY = jumpSpeed; onGround = false;}
    // if(keys[SDL_SCANCODE_S]){y+= moveSpeed;} 
    if(keys[SDL_SCANCODE_A]){velocityX -= moveSpeed; lastDirection = Direction::LEFT;}
    if(keys[SDL_SCANCODE_D]){velocityX += moveSpeed; lastDirection = Direction::RIGHT;}

    if (!onGround) {
    velocityY += gravity;
    }

    x += velocityX;
    y += velocityY;




    if (x < 0) x = 0;
    if (y < 0) y = 0;
    // if (x + width > constants::gScreenWidth) x = constants::gScreenWidth - width;
    if (y + height > constants::gScreenHeight) {y = constants::gScreenHeight - height; velocityY = 0; onGround = true;}
}
