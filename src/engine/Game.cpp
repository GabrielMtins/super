#include "engine/Game.hpp"

Game::Game(void) {
	dt = 0.0f;
	current_tick = 0;
}

void Game::init(const std::string& title, int internal_width, int internal_height) {
	context.init(
			title,
			internal_width,
			internal_height
			);

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

bool Game::getKeyDown(InputType input_type) {
	return input_tick_down[input_type] == current_tick;
}
bool Game::getKeyUp(InputType input_type) {
	return input_tick_up[input_type] == current_tick;
}

void Game::setKeyInput(InputType input_type, int scancode) {
	input_to_keys[input_type] = scancode;
}

Tick Game::getCurrentTick(void) {
	return current_tick;
}

void Game::quit(void) {
	resource_manager.quit();
	context.quit();
}

void Game::loop(void) {
	uint32_t new_tick;

	context.pollEvents();
	updateKeyState();

	entity_list.update(this, dt);

	context.renderClear(0x00, 0x00, 0x00, 0xff);
	entity_list.render(this);
	world.render(this, 0);
	context.renderPresent();

	new_tick = SDL_GetTicks();
	dt = (float) (new_tick - current_tick) / 1000.0f;
	current_tick = new_tick;
}

void Game::updateKeyState(void) {
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < NUM_INPUTS; i++) {
		bool new_state;

		new_state = keys[input_to_keys[i]];

		if(new_state == true && pressed[i] == false) {
			input_tick_down[i] = current_tick;
		}
		else if(new_state == false && pressed[i] == true) {
			input_tick_up[i] = current_tick;
		}

		pressed[i] = new_state;
	}
}
