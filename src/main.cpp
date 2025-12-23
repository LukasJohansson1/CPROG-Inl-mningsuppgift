#include "GameEngine.h"
#include "Sprite.h"
#include "Constants.h"
#include <memory>

int main() {
	GameEngine engine(60);
	if (!engine.init()) return 1;


	engine.run();
	return 0;

	
}