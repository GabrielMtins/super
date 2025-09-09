#include "game/CustomEntities.hpp"

namespace Bullet {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_BULLET;
		entity->size = Vec2(5.0f, 5.0f);
		entity->sprite.setTexture(game->getTexture("bullet"));

		entity->collision_mask |= COLLISIONLAYER_STATIC;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;

		entity->alive = false;
	}
}

EntityHandler Bullet_GetHandler(void) {
	using namespace Bullet;
	return EntityHandler(create, update, collision);
}
