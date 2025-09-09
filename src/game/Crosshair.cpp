#include "game/CustomEntities.hpp"

namespace Crosshair {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->sprite.setTexture(game->getTexture("crosshair"));
		entity->sprite.offset = entity->sprite.size / 2;
		entity->sprite.hud_element = false;

		entity->type = ENTITY_CROSSHAIR;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		entity->position = game->getMousePosition() + game->getCameraPosition();
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Crosshair_GetHandler(void) {
	using namespace Crosshair;
	return EntityHandler(create, update, collision);
}
