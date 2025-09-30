#include "game/CustomEntities.hpp"

namespace Jumper {
	enum JumperStates {
		STATE_FLOOR = 0,
		STATE_JUMP
	};

	enum JumperFlags {
		FLAG_ONFLOOR = 0,
	};

	enum JumperTimers {
		TIMER_CANJUMP = 0,
	};

	static const Tick down_time = 700;
	static const float jump_velocity = -200.0f;
	static const float gravity_multiplier = 0.7f;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_JUMPER;

		entity->hitbox.layer |= COLLISIONLAYER_ENEMY;
		entity->hitbox.layer |= COLLISIONLAYER_THROWABLE;
		entity->hitbox.mask |= COLLISIONLAYER_STATIC;

		entity->hitbox.size = Vec2(8.0f, 8.0f);
		entity->sprite.flip_y = false;
		entity->sprite.offset = Vec2(2.0f, 4.0f);

		entity->sprite.setTexture(game->getTexture("walker"));
		entity->velocity.x = 0.0f;

		entity->health = 0;

		entity->state = STATE_JUMP;
		entity->flags[FLAG_ONFLOOR] = false;

		entity->only_update_when_visible = true;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		if(entity->velocity.y != 0.0f) {
			entity->direction.y = entity->velocity.y;
		}

		switch(entity->state) {
			case STATE_FLOOR:
				if(game->getCurrentTick() > entity->timers[TIMER_CANJUMP]) {
					entity->state = STATE_JUMP;
					entity->velocity.y = jump_velocity;
					entity->flags[FLAG_ONFLOOR] = false;
				}

				break;

			case STATE_JUMP:
				if(entity->flags[FLAG_ONFLOOR]) {
					entity->timers[TIMER_CANJUMP] = game->getCurrentTick() + down_time;
					entity->state = STATE_FLOOR;
					entity->flags[FLAG_ONFLOOR] = false;
				}

				break;
		}
		
		entity->velocity += gravity * dt * gravity_multiplier;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;

		if(entity->velocity.y == 0.0f && entity->direction.y > 0.0f) {
			entity->flags[FLAG_ONFLOOR] = true;
		}
	}
}

EntityHandler Jumper_GetHandler(void) {
	using namespace Jumper;
	return EntityHandler(create, update, collision);
}
