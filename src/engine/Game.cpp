#include "engine/Game.hpp"

Game::Game(void) {
	context.init();
	resource_manager.load(&context, "res/res.json");

	setKeyInput(INPUT_LEFT, SDL_SCANCODE_A);
	setKeyInput(INPUT_RIGHT, SDL_SCANCODE_D);
	setKeyInput(INPUT_DOWN, SDL_SCANCODE_S);
	setKeyInput(INPUT_UP, SDL_SCANCODE_W);
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

bool Game::getKey(InputType input_type) {
	return pressed[input_type];
}

void Game::setKeyInput(InputType input_type, int scancode) {
	input_to_keys[input_type] = scancode;
}

Game::~Game(void) {
	resource_manager.quit();
	context.quit();
}

void Game::loop(void) {
	context.pollEvents();

	updateKeyState();

	entity_list.update(this);

	context.renderClear(0x00, 0x00, 0x00, 0xff);
	entity_list.render(this);
	context.renderPresent();
}

void Game::updateKeyState(void) {
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < NUM_INPUTS; i++) {
		pressed[i] = keys[input_to_keys[i]];
	}
}
