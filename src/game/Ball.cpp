#include "game/CustomEntities.hpp"

namespace Ball {
	const static int sprite_cell = 2;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_BALL;

		entity->sprite.setTexture(game->getTexture("8x8-items"));
		entity->sprite.cell = sprite_cell;
		entity->hitbox.size = Vec2(8.0f, 8.0f);

		entity->hitbox.layer |= COLLISIONLAYER_STATIC;
		entity->hitbox.layer |= COLLISIONLAYER_THROWABLE;

		entity->hitbox.mask |= COLLISIONLAYER_STATIC;
		entity->velocity = Vec2();
		entity->sprite.flip_y = false;
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

EntityHandler Ball_GetHandler(void) {
	using namespace Ball;
	return EntityHandler(create, update, collision);
}
