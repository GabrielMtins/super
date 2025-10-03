#include "game/CustomEntities.hpp"

namespace MenuHandler {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_MENUHANDLER;
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

EntityHandler MenuHandler_GetHandler(void) {
	using namespace MenuHandler;
	return EntityHandler(create, update, collision);
}
