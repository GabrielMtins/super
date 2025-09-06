#include "engine/Game.hpp"

Game::Game(void) {
	context.init();
	resource_manager.load(&context, "res/res.json");
}

void Game::run(void) {
	while(context.isRunning()) {
		loop();
	}
}

Context * Game::getContext(void) {
	return &context;
}

EntityList * Game::getEntityList(void) {
	return &entity_list;
}

Texture * Game::getTexture(const std::string& name) {
	return resource_manager.getTexture(name);
}

Game::~Game(void) {
	resource_manager.quit();
	context.quit();
}

void Game::loop(void) {
	context.pollEvents();

	entity_list.update(this);

	context.renderClear(0x00, 0x00, 0x00, 0xff);
	entity_list.render(this);
	context.renderPresent();
}
