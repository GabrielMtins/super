#include "game/CustomEntities.hpp"

namespace Zombie {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_ZOMBIE;
		entity->sprite.setTexture(game->getTexture("walker"));
		entity->sprite.offset = Vec2(4.0f, 8.0f);

		entity->size = Vec2(16.0f, 16.0f);

		entity->collision_mask |= COLLISIONLAYER_STATIC;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;
		Vec2 direction;
		Entity *player;

		if(entity->target_id < 0) {
			EntityFoundList found_list = game->findEntity(ENTITY_PLAYER);

			if(found_list.size() > 0)
				entity->target_id = found_list.at(0);
			
			return;
		}

		player = game->getEntityFromId(entity->target_id);

		direction = player->position - entity->position;

		entity->velocity = direction.normalized() * 80.0f;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Zombie_GetHandler(void) {
	using namespace Zombie;
	return EntityHandler(create, update, collision);
}
