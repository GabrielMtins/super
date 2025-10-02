#include "game/CustomEntities.hpp"

namespace Background {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_BACKGROUND;
		entity->sprite.layer = RENDERLAYER_BACKGROUND;
		entity->sprite.setTexture(game->getTexture("background-city"));
		entity->sprite.ignore_camera = true;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		entity->hitbox.position.x = -game->getCameraPosition().x * 0.8f;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Background_GetHandler(void) {
	using namespace Background;
	return EntityHandler(create, update, collision);
}
