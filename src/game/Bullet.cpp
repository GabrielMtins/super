#include "game/CustomEntities.hpp"

namespace Bullet {
	const int KILL_TIMER = 0;
	const Tick time_to_die = 5000;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_BULLET;
		entity->size = Vec2(5.0f, 5.0f);
		entity->sprite.setTexture(game->getTexture("bullet"));

		entity->collision_mask |= COLLISIONLAYER_STATIC;

		entity->timers[KILL_TIMER] = game->getCurrentTick();
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		if(game->getCurrentTick() - entity->timers[KILL_TIMER] > time_to_die) {
			entity->alive = false;
		}
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
