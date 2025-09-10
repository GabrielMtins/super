#include "game/CustomEntities.hpp"

namespace ExampleEntity {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_EXAMPLEENTITY;
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

EntityHandler ExampleEntity_GetHandler(void) {
	using namespace ExampleEntity;
	return EntityHandler(create, update, collision);
}
