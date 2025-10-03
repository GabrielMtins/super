#include "game/CustomEntities.hpp"
#include "game/EnemyAI.hpp"

namespace KeyItem {
	static const int sprite_cell = 5;
	static const float speed = 150.0f;
	static const float deacceleration = 500.0f;
	static const float jump_speed = -150.0f;

	enum KeyItemStates {
		STATE_START,
		STATE_GOUP,
		STATE_FOLLOWPLAYER,
		STATE_FLOAT,
	};

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_KEYITEM;

		entity->sprite.setTexture(game->getTexture("8x8-items"));
		entity->sprite.cell = sprite_cell;
		entity->sprite.flip_y = 0;

		entity->hitbox.size = Vec2(8.0f, 3.0f);

		entity->velocity = Vec2::zero;

		entity->hitbox.layer = COLLISIONLAYER_ITEMS;
		entity->hitbox.mask = 0;

		entity->direction.y = 1.0f;
		entity->state = STATE_START;
	}

	static void findPlayer(Game *game, Entity *entity) {
		EntityFoundList list = game->findEntity(ENTITY_PLAYER);

		if(list.empty())
			return;

		entity->target_id = list.at(0);
	}

	static Vec2 getPlayerDirection(Game *game, Entity *entity) {
		Entity *player;

		player = game->getEntityFromId(entity->target_id);

		if(player == NULL)
			return Vec2::zero;

		return (player->center - entity->center).normalized();
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		switch(entity->state) {
			case STATE_FLOAT:
				break;

			case STATE_START:
				findPlayer(game, entity);

				if(entity->target_id) {
					entity->state = STATE_GOUP;
					entity->velocity.y = jump_speed;
				}
				break;

			case STATE_GOUP:
				entity->velocity.y += deacceleration * dt;

				if(entity->velocity.y > 0.0f) {
					entity->velocity.y = 0.0f;
					entity->state = STATE_FOLLOWPLAYER;
				}
				
				break;

			case STATE_FOLLOWPLAYER:
				if(EnemyAI::checkCollisionThisFrame(game)) {
					entity->velocity = getPlayerDirection(game, entity) * speed;
				}
				break;
		}
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler KeyItem_GetHandler(void) {
	using namespace KeyItem;
	return EntityHandler(create, update, collision);
}
