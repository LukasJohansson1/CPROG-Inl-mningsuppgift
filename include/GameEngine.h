#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include <functional>
#include "Sprite.h"
#include "Constants.h"

class GameEngine {

public:
    GameEngine(int fps = 60);
    ~GameEngine();

    bool init();
    void run();

    // Sprite hantering
    void addSprite(std::shared_ptr<Sprite> sprite);
    void removeSprite(std::shared_ptr<Sprite> sprite);
    const std::vector<std::shared_ptr<Sprite>>& getAllSprites() const { return sprites; }

    // Callback för speluppdatering
    using GameUpdateCallback = std::function<void(bool&)>;
    void setGameUpdateCallback(GameUpdateCallback callback) { gameUpdateCallback = callback; }

    // Rendering tillgång för spelklassen
    SDL_Renderer* getRenderer() const { return renderer; }
    float getCameraX() const { return cameraX; }

private:
    int fps;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* bgtexture = nullptr;
    std::vector<std::shared_ptr<Sprite>> sprites;
    float cameraX = 0.0f;

    GameUpdateCallback gameUpdateCallback;

    void updateCameraAndGround();
    bool handleEvents();
    void showHealthBar();
    void render();
};