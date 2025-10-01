#include "game/CustomEntities.hpp"

namespace ItemBox {
	static const int sprite_cell = 0;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_ITEMBOX;

		entity->sprite.setTexture(game->getTexture("8x8-items"));
		entity->sprite.flip_y = false;
		entity->sprite.cell = sprite_cell;

		entity->hitbox.size = Vec2(8.0f, 8.0f);

		entity->hitbox.layer |= COLLISIONLAYER_STATIC;
		entity->hitbox.layer |= COLLISIONLAYER_THROWABLE;

		entity->hitbox.mask |= COLLISIONLAYER_STATIC;
		entity->velocity = Vec2();
		entity->sprite.flip_y = false;
		entity->health = 1;

		entity->alt_type = ENTITY_KEYITEM;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		entity->velocity += gravity * dt;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler ItemBox_GetHandler(void) {
	using namespace ItemBox;
	return EntityHandler(create, update, collision);
}
