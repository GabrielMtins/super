#include "game/Scenes.hpp"

#include <vector>
#include <string>

namespace Scene {
	static const std::vector<std::string> levels = {
		"res/levels/map01.tmj"
	};

	static int next_level = 0;

	void titleScreen(Game *game) {
	}

	void levelScreen(Game *game) {
		game->loadWorld(levels[next_level]);
		game->addEntity(ENTITY_PLAYER);
		game->addEntity(ENTITY_CAMERA);
		game->addEntity(ENTITY_BACKGROUND);
		game->addEntity(ENTITY_HUD);
	}
};
