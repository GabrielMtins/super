#include "game/CustomEntities.hpp"

namespace Camera {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_CAMERA;
	}

	static void findPlayer(Game *game, Entity *entity) {
		if(!entity->target_id) {
			EntityFoundList list = game->findEntity(ENTITY_PLAYER);

			if(list.size() > 0) {
				entity->target_id = list.at(0);
			}
		}
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		Entity *player;

		findPlayer(game, entity);

		if(!entity->target_id)
			return;

		player = game->getEntityFromId(entity->target_id);

		if(!player)
			return;

		game->setCameraPosition(
				Vec2(
					player->center.x - 100.0f,
					-16.0f
					)
				);
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Camera_GetHandler(void) {
	using namespace Camera;
	return EntityHandler(create, update, collision);
}
