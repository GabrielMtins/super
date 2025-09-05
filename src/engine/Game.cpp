#include "engine/Game.hpp"

Game::Game(void) {
	texture.load(&context, "character.png");
	texture.setCellSize(24, 24);
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

Game::~Game(void) {
}

void Game::loop(void) {
	context.pollEvents();

	entity_list.update(this);

	context.renderClear(0x00, 0x00, 0x00, 0xff);
	entity_list.render(this);
	context.renderPresent();
}
