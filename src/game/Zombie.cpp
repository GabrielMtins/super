#include "game/CustomEntities.hpp"

namespace Zombie {
	static const int walking_animation[] = {0, 1, -1};
	static const int idle_animation[] = {0, -1};

	static const float roaming_speed = 60.0f;

	static const int roaming_timer_id = 0;
	static const int roaming_think_delay = 1000;

	enum States {
		ROAMING = 0,
		CHASING
	};

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_ZOMBIE;
		entity->sprite.setTexture(game->getTexture("walker"));
		entity->sprite.offset = Vec2(4.0f, 8.0f);

		entity->size = Vec2(16.0f, 16.0f);

		entity->collision_mask |= COLLISIONLAYER_STATIC;

		entity->state = ROAMING;
	}

	static void roaming(Game *game, Entity *entity, float dt) {
		Tick& roaming_timer = entity->timers[roaming_timer_id];

		if(game->getCurrentTick() < roaming_timer) {
			return;
		}

		roaming_timer = game->getCurrentTick() + roaming_think_delay;

		if(rand() % 2 == 0) {
			entity->velocity = Vec2::zero;
			entity->animator.setAnimation(idle_animation, 200);
		} else {
			Vec2 direction(rand() % 1000 - 500, rand() % 1000 - 500);

			entity->velocity = direction.normalized() * roaming_speed;
			entity->animator.setAnimation(walking_animation, 200);
		}
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
		}

		player = game->getEntityFromId(entity->target_id);

		if(!player)
			return;

		direction = player->position - entity->position;

		roaming(game, entity, dt);
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;

		if(other != NULL && other->type == ENTITY_PLAYER) {
			if(other->getDamage(game, 20)) {
				printf("ouch\n");
			}
		}
	}
}

EntityHandler Zombie_GetHandler(void) {
	using namespace Zombie;
	return EntityHandler(create, update, collision);
}
