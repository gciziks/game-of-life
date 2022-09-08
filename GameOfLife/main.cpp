#include "Game.h"

int main() {

	//Init Engine
	Game game;

	while (game.getWindowIsOpen()) {
		
		game.update();

		game.render();
	}
	return 0;
}