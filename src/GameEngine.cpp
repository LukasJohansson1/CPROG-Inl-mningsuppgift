#include <GameEngine.h>
#include <algorithm>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <cmath>


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

void GameEngine::spawnEnemy() {
    float playerX = 0.0f;
    for (auto& sprite : sprites) {
        if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
            playerX = player->getX();
            break;
        }
    }

    float enemyX = playerX + constants::gScreenWidth - 170.0f;
    float minY = 100.0f;
    float maxY = constants::gScreenHeight - 150.0f;
    float enemyY = minY + static_cast<float>(rand()) / RAND_MAX * (maxY - minY);

    addSprite(std::make_shared<Enemy>(enemyX, enemyY));
}

void GameEngine::spawnLedge(){
    constexpr float ledgeWidth = 110.0f;
    constexpr float ledgeHeight = 15.0f;

    //Undre ledge 
    float x_lower = lastLedgeX + 100.0f + static_cast<float>(rand()) / RAND_MAX * 120.0f;
    float y_lower = constants::gScreenHeight - 150.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f; // nära botten med liten variation
    addSprite(std::make_shared<Ledge>(x_lower, y_lower, ledgeWidth, ledgeHeight));

    // Övre ledge
    float x_upper = x_lower + 80.0f + static_cast<float>(rand()) / RAND_MAX * 60.0f;
    float y_upper = constants::gScreenHeight / 3.0f + static_cast<float>(rand()) / RAND_MAX * (constants::gScreenHeight / 3.0f); // högre med större variation
    addSprite(std::make_shared<Ledge>(x_upper, y_upper, ledgeWidth, ledgeHeight));

    // Spara positioner för nästa spawn
    lastLedgeX = x_upper;
    lastLedgeY = y_lower;

    // Begränsa antalet ledges
    std::vector<std::shared_ptr<Ledge>> ledges;
    for (auto& sprite : sprites) {
        if (auto l = std::dynamic_pointer_cast<Ledge>(sprite)) {
            if (l->getY() != constants::gScreenHeight - 2.0f)
                ledges.push_back(l);
        }
    }
    if (ledges.size() > 12) removeSprite(ledges.front());
    }
    


void GameEngine::run() {
    bool running = true;
    Uint64 ticksPerFrame = 1000 / fps;

    Uint64 lastLedgeSpawn = SDL_GetTicks();
    Uint64 lastEnemySpawn = SDL_GetTicks();

    while (running) {
        Uint64 frameStart = SDL_GetTicks();
        Uint64 now = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) { // Hantera händelser
            if (event.type == SDL_EVENT_QUIT) running = false;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
                for (auto& sprite : sprites) {
                    if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                        float projSpeed = 6.0f;
                        float speedX = (player->getLastDirection() == Direction::LEFT ? -projSpeed : projSpeed);

                        auto proj = std::make_shared<Projectile>(
                            player->getX() + player->getWidth() / 2 - 5,
                            player->getY() + player->getHeight() / 2 - 5,
                            speedX, 0.0f
                        );
                        addSprite(proj);
                    }
                }
            }
        }

        for (auto& sprite : sprites) sprite->tick();

        // Enemy spawnas varje 2 sekunder
        if (now - lastEnemySpawn >= 2000) {
            lastEnemySpawn = now;
            spawnEnemy();
        }

        // Ledge spawnas varje 0.8 sekunder
        if (now - lastLedgeSpawn >= 800) {
            lastLedgeSpawn = now;
            spawnLedge();
        }

        // Projectile kollision med enemies
        for (auto& sprite : sprites) { 
            if (auto proj = std::dynamic_pointer_cast<Projectile>(sprite)) {
                SDL_FRect pRect = proj->getRect();
                for (auto& target : sprites) {
                    if (auto enemy = std::dynamic_pointer_cast<Enemy>(target)) {
                        if (intersects(pRect, enemy->getRect())) {
                            enemy->destroy();
                            proj->destroy();
                        }
                    }
                }
            }
        }

        // Player kollision med enemies 
        for (auto& sprite : sprites) {
            if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                SDL_FRect p = player->getRect();

                for (auto& other : sprites) {
                    if (auto enemy = std::dynamic_pointer_cast<Enemy>(other)) {
                        if (intersects(p, enemy->getRect())) {
                            player->takeDamage(10);
                            player->setHealth(std::clamp(player->getHealth(), 0.0f, 100.0f));
                            enemy->destroy();

                            //  En gameover check för att avsluta spel när du dör
                            if (player->getHealth() <= 0.0f) {
                                SDL_ShowSimpleMessageBox(
                                    SDL_MESSAGEBOX_INFORMATION,
                                    "Game Over",
                                    "Du dog!",
                                    window
                                );
                                return; // Avslutar run() direkt
                            }
                        }
                    }
                }
            }
        }

        // Player health regeneration
        for (auto& sprite : sprites) {
            if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                player->regenerateHealth(0.02f);
            }
        }

        //  Player kollision med ledges 
        for (auto& sprite : sprites) {
            if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                player->setOnGround(false);
                SDL_FRect p = player->getRect();

                for (auto& other : sprites) {
                    if (auto ledge = std::dynamic_pointer_cast<Ledge>(other)) {
                        SDL_FRect l = ledge->getRect();

                        if (intersects(p, l)) {
                            if (player->getVelocityX() > 0 && p.x + p.w - player->getVelocityX() <= l.x)
                                player->setX(l.x - p.w);
                            else if (player->getVelocityX() < 0 && p.x - player->getVelocityX() >= l.x + l.w)
                                player->setX(l.x + l.w);
                        }

                        bool horizontalOverlap = p.x + p.w > l.x && p.x < l.x + l.w;
                        if (horizontalOverlap && player->getVelocityY() >= 0 && p.y + p.h <= l.y && p.y + p.h + player->getVelocityY() >= l.y) {
                            player->setY(l.y - p.h);
                            player->setVelocityY(0);
                            player->setOnGround(true);
                        }
                    }
                }
            }
        }

        sprites.erase(std::remove_if(sprites.begin(), sprites.end(),
                                     [](const std::shared_ptr<Sprite>& s){ return !s->isAlive(); }),
                      sprites.end());

        for (auto& sprite : sprites) {
            if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                cameraX = std::max(0.0f, player->getX() - constants::gScreenWidth / 4.0f);
            }
        }

        
        for (auto& sprite : sprites) {
            if (auto ledge = std::dynamic_pointer_cast<Ledge>(sprite)) {
                if (ledge->getY() == constants::gScreenHeight - 2.0f) {
                    ledge->setX(cameraX - 500.0f);
                    ledge->setWidth(constants::gScreenWidth + 1000.0f);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        float bgWidth = constants::gScreenWidth;
        float bgOffsetX = fmod(cameraX, bgWidth);
        for (int i = -1; i <= 2; i++) {
            SDL_FRect bgRect{i * bgWidth - bgOffsetX, 0, bgWidth, constants::gScreenHeight};
            SDL_RenderTexture(renderer, bgtexture, nullptr, &bgRect);
        }

        for (auto& sprite : sprites) { 
            SDL_FRect rect = sprite->getRect();
            rect.x -= cameraX;
            SDL_SetRenderDrawColor(renderer, sprite->getColor().r, sprite->getColor().g, sprite->getColor().b, sprite->getColor().a);
            SDL_RenderFillRect(renderer, &rect);
        }

        // Ritar ut healthbar
        for (auto& sprite : sprites) {
            if (auto player = std::dynamic_pointer_cast<Player>(sprite)) {
                float health = player->getHealth();
                float healthBarWidth = 100.0f;
                float healthPercent = std::clamp(health / 100.0f, 0.0f, 1.0f);

                // Röd bakgrund först
                SDL_FRect healthBarBg{20.0f, 20.0f, healthBarWidth, 20.0f};
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &healthBarBg);

                // Grön fyllning ovanpå
                SDL_FRect healthBar{20.0f, 20.0f, healthBarWidth * healthPercent, 20.0f};
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &healthBar);

            }
        }
        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < ticksPerFrame) SDL_Delay(ticksPerFrame - frameTime);
    }
}