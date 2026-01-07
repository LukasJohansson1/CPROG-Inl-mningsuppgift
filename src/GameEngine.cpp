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

    auto ground = std::make_shared<Ledge>(0.0f,constants::gScreenHeight - 2.0f,constants::gScreenWidth,2.0f); // Small ledge at the bottom
    addSprite(ground);

    return true;
}

void GameEngine::addSprite(std::shared_ptr<Sprite> sprite) {
    sprites.push_back(sprite);
}

void GameEngine::removeSprite(std::shared_ptr<Sprite> sprite) {
    sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}

bool intersects(const SDL_FRect& a, const SDL_FRect& b) {
    return( a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
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
                            float projSpeed = 6.0f;
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

        for(auto& sprite : sprites) {
            if(auto player = std::dynamic_pointer_cast<Player>(sprite)){
                player->setOnGround(false);

                SDL_FRect p = player->getRect();

                for(auto& other : sprites) {
                    if(auto ledge = std::dynamic_pointer_cast<Ledge>(other)) {
                        SDL_FRect l = ledge->getRect();

                        if(intersects(p,l)){ // Collision from left
                            if(player->getVelocityX() > 0 && p.x + p.w - player->getVelocityX() <= l.x) {
                                player->setX(l.x - p.w);
                            }

                            else if(player->getVelocityX() < 0 && p.x -player->getVelocityX() >= l.x + l.w) {
                                player->setX(l.x + l.w);
                            }
                        }


                        bool horizontalOverlap = p.x + p.w > l.x && p.x < l.x + l.w;

                        if(horizontalOverlap && player->getVelocityY() >= 0 && p.y + p.h <= l.y && p.y + p.h + player->getVelocityY() >= l.y) {{
                            player->setY(l.y - p.h);
                            player->setVelocityY(0);
                            player->setOnGround(true);
                        }
                        }
                    }
                }
            }
        }

        sprites.erase(std::remove_if(sprites.begin(), sprites.end(), [](const std::shared_ptr<Sprite>& s){return !s->isAlive();}), sprites.end()); // påminn ändra till spriteRemove

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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