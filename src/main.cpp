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


	engine.run();
	return 0;

	
}