#include "game/CustomEntities.hpp"

namespace Lifebar {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_LIFEBAR;

		entity->sprite.setTexture(game->getTexture("lifebar"));
		entity->sprite.hud_element = true;
		entity->sprite.ignore_camera = true;

		entity->hitbox.position = Vec2(2.0f, 2.0f);
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;
		
		Entity *player;

		if(!entity->target_id) {
			EntityFoundList list = game->findEntity(ENTITY_PLAYER);

			if(list.size() > 0)
				entity->target_id = list.at(0);
		}

		player = game->getEntityFromId(entity->target_id);

		if(!player)
			return;

		entity->sprite.cell = player->health - 1;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Lifebar_GetHandler(void) {
	using namespace Lifebar;
	return EntityHandler(create, update, collision);
}
