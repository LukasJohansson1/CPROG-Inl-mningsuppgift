// SPELKlass

#include "Game.h"

int main() {
	Game game(60);
	if (!game.init()) {
		return 1;
	}

	game.run();
	return 0;
}