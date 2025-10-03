#include "game/CustomEntities.hpp"

namespace Camera {
	enum CameraStates {
		STATE_START,
		STATE_RUN
	};

	/* Depende da resolução, fazendo para uma resolução de 240x135 */
	static const float boundary_left = 100.0f;
	static const float boundary_right = 110.0f;

	static const float boundary_bottom = 30.0f;
	static const float boundary_top = 119.0f;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_CAMERA;

		entity->state = STATE_START;
	}

	static void followPlayer(Game *game, Entity *entity) {
		Entity *player;

		player = game->getEntityFromId(entity->target_id);

		const Vec2& center = player->center;
		Vec2 camera = game->getCameraPosition();

		if(player->health <= 1)
			return;

		float player_bottom = player->hitbox.position.y;
		float player_top = player_bottom + player->hitbox.size.y;

		if(center.x < camera.x + boundary_left) {
			camera.x = center.x - boundary_left;
		} else if(center.x > camera.x + boundary_right) {
			camera.x = center.x - boundary_right;
		}

		if(camera.x < 0.0f)
			camera.x = 0.0f;

		/*
		if(player_bottom < camera.y + boundary_bottom) {
			camera.y = player_bottom - boundary_bottom;
		} else if(player_top > camera.y + boundary_top) {
			camera.y = player_top - boundary_top;
		}
		*/

		game->setCameraPosition(camera);
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		switch(entity->state) {
			case STATE_START:
				{
					EntityFoundList list = game->findEntity(ENTITY_PLAYER);

					if(!list.empty()) {
						entity->target_id = list.at(0);
						entity->state = STATE_RUN;
					}
					
					game->setCameraPosition(Vec2(0.0f, -16.0f));
				}
				break;

			case STATE_RUN:
				followPlayer(game, entity);
				break;
		}

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
