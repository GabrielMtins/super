#include "engine/Game.hpp"

Game::Game(void) {
	context.init();

	setKeyInput(INPUT_LEFT, SDL_SCANCODE_A);
	setKeyInput(INPUT_RIGHT, SDL_SCANCODE_D);
	setKeyInput(INPUT_DOWN, SDL_SCANCODE_S);
	setKeyInput(INPUT_UP, SDL_SCANCODE_W);

	dt = 0.0f;
	current_tick = 0;
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

World * Game::getWorld(void) {
	return &world;
}

void Game::loadRes(const std::string& filename) {
	resource_manager.load(&context, filename);
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
	uint32_t new_tick;
	current_tick = SDL_GetTicks();

	context.pollEvents();

	updateKeyState();

	entity_list.update(this, dt);

	context.renderClear(0x00, 0x00, 0x00, 0xff);
	entity_list.render(this);
	world.render(this, 0);
	context.renderPresent();

	new_tick = SDL_GetTicks();
	dt = (float) (new_tick - current_tick) / 1000.0f;
}

void Game::updateKeyState(void) {
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < NUM_INPUTS; i++) {
		pressed[i] = keys[input_to_keys[i]];
	}
}
