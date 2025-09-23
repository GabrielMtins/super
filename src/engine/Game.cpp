#include "engine/Game.hpp"
#include <iterator>

Game::Game(void) {
	dt = 0.01f;
	current_tick = 0;
}

void Game::init(const std::string& title, int internal_width, int internal_height) {
	context.init(
			title,
			internal_width,
			internal_height
			);

	for(int i = 0; i < NUM_INPUTS; i++) {
		input_to_keys[i] = SDL_SCANCODE_UNKNOWN;
	}

	setKeyInput(INPUT_LEFT, SDL_SCANCODE_A);
	setKeyInput(INPUT_RIGHT, SDL_SCANCODE_D);
	setKeyInput(INPUT_DOWN, SDL_SCANCODE_S);
	setKeyInput(INPUT_UP, SDL_SCANCODE_W);
	setKeyInput(INPUT_FIRE, SDL_SCANCODE_J);

	screen_dimensions = Vec2(internal_width, internal_height);

	dt = 0.0f;
	current_tick = 0;
	paused = false;

	for(Tick& tick : input_tick_down) 
		tick = 999999;

	for(Tick& tick : input_tick_up) 
		tick = 999999;

	for(bool& press : pressed) {
		press = false;
	}

	mouse_state = 0;
	old_mouse_state = 0;
}

void Game::run(void) {
	while(context.isRunning()) {
		loop();
	}
}

Context * Game::getContext(void) {
	return &context;
}

World * Game::getWorld(void) {
	return &world;
}

void Game::loadRes(const std::string& filename) {
	resource_manager.load(&context, filename);
}

void Game::loadFont(const std::string& filename, int size) {
	text_generator.loadFont(filename, size);
}

void Game::loadLocale(const std::string& filename, const std::string& locale_name) {
	text_generator.loadLocale(&context, filename, locale_name);
}

void Game::loadWorld(const std::string& filename) {
	world.load(
			&context,
			filename,
			this
			);
}

Texture * Game::getTexture(const std::string& name) {
	return resource_manager.getTexture(name);
}

Sfx * Game::getSfx(const std::string& name) {
	return resource_manager.getSfx(name);
}

Music * Game::getMusic(const std::string& name) {
	return resource_manager.getMusic(name);
}

const Vec2& Game::getCameraPosition(void) const {
	return camera_position;
}

void Game::setCameraPosition(const Vec2& camera_position) {
	this->camera_position = camera_position;
}

void Game::pause(void) {
	paused = !paused;
}

bool Game::isPaused(void) {
	return paused;
}

const Vec2& Game::getScreenDimensions(void) const {
	return screen_dimensions;
}

const Vec2& Game::getMousePosition(void) const {
	return mouse_position;
}

bool Game::getKey(InputType input_type) const {
	return pressed[input_type];
}

bool Game::getKeyDown(InputType input_type) const {
	return input_tick_down[input_type] == current_tick;
}

bool Game::getKeyUp(InputType input_type) const {
	return input_tick_up[input_type] == current_tick;
}

bool Game::getMouseButton(MouseButton mouse_button) const {
	return mouse_state & SDL_BUTTON(mouse_button);
}
bool Game::getMouseButtonDown(MouseButton mouse_button) const {
	return (mouse_state & SDL_BUTTON(mouse_button)) && 
		!(old_mouse_state & SDL_BUTTON(mouse_button));
}
bool Game::getMouseButtonUp(MouseButton mouse_button) const {
	return !(mouse_state & SDL_BUTTON(mouse_button)) && 
		(old_mouse_state & SDL_BUTTON(mouse_button));
}

void Game::setKeyInput(InputType input_type, int scancode) {
	input_to_keys[input_type] = scancode;
}

Tick Game::getCurrentTick(void) const {
	return current_tick;
}

void Game::addHandlerToType(EntityType type, const EntityHandler& handler) {
	entity_list.addHandlerToType(type, handler);
}

EntityId Game::addEntity(EntityType type) {
	return entity_list.addEntity(this, type);
}

Entity * Game::getEntityFromId(EntityId id) {
	return entity_list.getEntityFromId(id);
}

EntityFoundList Game::findEntity(const Vec2& position, float radius) {
	return entity_list.findEntity(position, radius);
}

EntityFoundList Game::findEntity(EntityType type) {
	return entity_list.findEntity(type);
}

EntityFoundList Game::findEntity(EntityType type, const Vec2& position, float radius) {
	return entity_list.findEntity(type, position, radius);
}

bool Game::checkCollision(const Hitbox& hitbox) const {
	return entity_list.checkCollision(&world, hitbox);
}

void Game::quit(void) {
	resource_manager.quit();
	text_generator.quit();
	context.quit();
}

void Game::loop(void) {
	uint32_t new_tick;

	context.pollEvents();
	updateKeyState();
	updateMouseState();

	entity_list.update(this, dt);

	context.renderClear(0x00, 0x00, 0x00, 0xff);

	for(int i = WORLD_LAYER_BG; i < WORLD_LAYER_FG; i++) {
		world.render(this, i);
	}

	entity_list.setSpriteRenderList(this, &sprite_renderer);
	sprite_renderer.renderSprites(this);

	world.render(this, WORLD_LAYER_FG);

	sprite_renderer.renderHud(this);
	sprite_renderer.reset();

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

void Game::updateMouseState(void) {
	mouse_position = Vec2(
			(float) context.getMouseX(),
			(float) context.getMouseY()
			);

	old_mouse_state = mouse_state;
	mouse_state = SDL_GetMouseState(NULL, NULL);
}
