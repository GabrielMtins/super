#include "game/CustomEntities.hpp"
#include "game/Player.hpp"

namespace Door {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_DOOR;

		entity->sprite.setTexture(game->getTexture("door"));
		entity->sprite.cell = 0;
		entity->hitbox.size = Vec2(8.0f, 24.0f);
		entity->sprite.layer = RENDERLAYER_ENTITIES;

		entity->hitbox.position = Vec2(32.0f, 32.0f);

		entity->hitbox.layer = COLLISIONLAYER_ITEMS;
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

		if(other == NULL)
			return;

		if(other->type != ENTITY_PLAYER)
			return;

		if(Player::hasKey(other)) {
			entity->alive = false;
			Player::useKey(other);
		}
	}
}

EntityHandler Door_GetHandler(void) {
	using namespace Door;
	return EntityHandler(create, update, collision);
}
