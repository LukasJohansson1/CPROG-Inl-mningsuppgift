#include "GameEngine.h"
#include "Sprite.h"
#include "Constants.h"
#include <memory>
#include "Player.h"

int main() {
	GameEngine engine(60);
	if (!engine.init()) return 1;

	auto player = std::make_shared<Player>(100,100);
	engine.addSprite(player);

	engine.addSprite(std::make_shared<Ledge>(100, 350, 200, 20));
	engine.addSprite(std::make_shared<Ledge>(400, 280, 180, 20));
	engine.addSprite(std::make_shared<Ledge>(200, 200, 150, 20));
	engine.addSprite(std::make_shared<Ledge>(500, 450, 150, 20));



	engine.run();
	return 0;

	
}