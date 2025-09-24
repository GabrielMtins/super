#include "game/CustomEntities.hpp"

namespace Platform {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_PLATFORM;
		entity->hitbox.type = Hitbox::COLLISION_ONEWAY_UP;
		entity->hitbox.layer = COLLISIONLAYER_STATIC;
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

EntityHandler Platform_GetHandler(void) {
	using namespace Platform;
	return EntityHandler(create, update, collision);
}
