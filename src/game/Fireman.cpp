#include "game/CustomEntities.hpp"

namespace Fireman {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_FIREMAN;
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

EntityHandler Fireman_GetHandler(void) {
	using namespace Fireman;
	return EntityHandler(create, update, collision);
}
