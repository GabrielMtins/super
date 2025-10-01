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

	setBackgroundColor(0xff, 0xff, 0xff);

	screen_dimensions = Vec2(internal_width, internal_height);
	
	setFps(200);
	setMinFps(60);

	dt = 0.0f;
	current_tick = 0;
	paused = false;

	mouse_state = 0;
	old_mouse_state = 0;

	frame_counter = 0;
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

bool Game::isPaused(void) const {
	return paused;
}

const Vec2& Game::getScreenDimensions(void) const {
	return screen_dimensions;
}

const Vec2& Game::getMousePosition(void) const {
	return mouse_position;
}

bool Game::getInput(int input_type) const {
	return input.getInput(input_type);
}

bool Game::getInputDown(int input_type) const {
	return input.getInputDown(input_type, current_tick);
}

bool Game::getInputUp(int input_type) const {
	return input.getInputUp(input_type, current_tick);
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

void Game::setInputKey(int input_type, int scancode) {
	input.setInputKey(input_type, scancode);
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

void Game::transformEntityToType(EntityId id, EntityType type) {
	entity_list.transformEntityToType(this, id, type);
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

EntityFoundList Game::findCollision(const Hitbox& hitbox) {
	return entity_list.findCollision(hitbox);
}

bool Game::checkCollision(const Hitbox& hitbox) const {
	return entity_list.checkCollision(&world, hitbox);
}

void Game::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b) {
	bg.r = r;
	bg.g = g;
	bg.b = b;
	bg.a = 0xff;
}

void Game::addSpriteToRenderList(const Sprite& sprite) {
	sprite_renderer.addSpriteToRenderList(this, sprite);
}

void Game::setFps(uint32_t fps) {
	delay_fps = 1000 / fps;
}

void Game::setMinFps(uint32_t fps) {
	minimum_dt = 1.0f / fps;
}

uint32_t Game::getCurrentFrame(void) const {
	return frame_counter;
}

void Game::quit(void) {
	resource_manager.quit();
	text_generator.quit();
	context.quit();
}

void Game::loop(void) {
	uint32_t new_tick;
	uint32_t delta_tick;

	context.pollEvents();
	input.update(current_tick);
	updateMouseState();

	entity_list.update(this, dt);

	context.renderClear(bg.r, bg.g, bg.b, bg.a);

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

	delta_tick = new_tick - current_tick;

	if(delta_tick < delay_fps) {
		SDL_Delay(delay_fps - delta_tick);
	}

	new_tick = SDL_GetTicks();
	delta_tick = new_tick - current_tick;

	dt = (float) (delta_tick) / 1000.0f;
	current_tick = new_tick;

	if(dt > minimum_dt) {
		dt = minimum_dt;
	}

	frame_counter++;
}

void Game::updateMouseState(void) {
	mouse_position = Vec2(
			(float) context.getMouseX(),
			(float) context.getMouseY()
			);

	old_mouse_state = mouse_state;
	mouse_state = SDL_GetMouseState(NULL, NULL);
}
