#include "game/CustomEntities.hpp"

namespace HeartItem {
	static const int sprite_cell = 1;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_HEARTITEM;

		entity->sprite.setTexture(game->getTexture("8x8-items"));
		entity->sprite.cell = sprite_cell;
		entity->sprite.flip_y = 0;

		entity->velocity = Vec2(0, -100.0f);

		entity->hitbox.layer = COLLISIONLAYER_ITEMS;
		entity->hitbox.mask = COLLISIONLAYER_STATIC;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		entity->velocity += gravity * dt * 0.3f;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler HeartItem_GetHandler(void) {
	using namespace HeartItem;
	return EntityHandler(create, update, collision);
}
