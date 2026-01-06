#include <GameEngine.h>
#include <algorithm>
#include <iostream>
#include <SDL3_image/SDL_image.h>

GameEngine::GameEngine(int fps) : fps(fps) {}

GameEngine::~GameEngine() {
    if(bgtexture) SDL_DestroyTexture(bgtexture);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool GameEngine::init() {
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("SDL 3 Game Engine", constants::gScreenWidth, constants::gScreenHeight,SDL_WindowFlags(0));

    if(!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    

    SDL_Surface* bgSurface = IMG_Load(constants::bg_str.c_str());
    if(!bgSurface) {
        std::cerr << "IMG_Load Error: " << SDL_GetError() << std::endl;
        return false;
    }


    bgtexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_DestroySurface(bgSurface); 
    if(!bgtexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void GameEngine::addSprite(std::shared_ptr<Sprite> sprite) {
    sprites.push_back(sprite);
}

void GameEngine::removeSprite(std::shared_ptr<Sprite> sprite) {
    sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}

void GameEngine::run(){
    bool running = true;
    Uint64 ticksPerFrame = 1000 / fps;

    while(running) {
        Uint64 frameStart = SDL_GetTicks();

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) running = false;

            if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if(event.button.button == SDL_BUTTON_LEFT) {
                    for (auto& sprite : sprites) {
                        if(auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                            float projSpeed = 4.0f;
                            float speedX = (player->getLastDirection() == Direction::LEFT ? -projSpeed : projSpeed);

                            auto proj = std::make_shared<Projectile>(player->getX()+ player->getWidth()/2 -5, player->getY() + player->getHeight()/2 -5, speedX, 0.0f);
                            addSprite(proj);
                        }
                    }
                }
            }
        }

        for (auto& sprite : sprites) {
            sprite->tick();
        }

        sprites.erase(std::remove_if(sprites.begin(), sprites.end(), [](const std::shared_ptr<Sprite>& s){return !s->isAlive();}), sprites.end());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Svart backgrund
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, bgtexture, nullptr, nullptr);


        for(auto& sprite : sprites) {
            sprite->draw(renderer);
        }

        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < ticksPerFrame) {
            SDL_Delay(ticksPerFrame - frameTime);
        }
    }
}