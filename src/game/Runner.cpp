#include "game/CustomEntities.hpp"

namespace Runner {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_RUNNER;
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
	}
}

EntityHandler Runner_GetHandler(void) {
	using namespace Runner;
	return EntityHandler(create, update, collision);
}
