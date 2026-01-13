#pragma once
#include <SDL3/SDL.h>

// Spelmotor input hantering

namespace input {
    inline bool isKeyDown(SDL_Scancode scancode) {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        return keys[scancode];
    }
}
