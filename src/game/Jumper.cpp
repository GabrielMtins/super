#include "game/CustomEntities.hpp"

namespace Jumper {
	enum JumperStates {
		STATE_FLOOR,
		STATE_JUMP,
		STATE_START
	};

	enum JumperFlags {
		FLAG_ONFLOOR
	};

	static const Tick down_time = 700;
	static const float jump_velocity = -150.0f;
	static const float gravity_multiplier = 0.5f;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_JUMPER;

		entity->hitbox.layer = 0;
		entity->hitbox.mask = 0;
		entity->hitbox.layer |= COLLISIONLAYER_ENEMY;
		entity->hitbox.layer |= COLLISIONLAYER_THROWABLE;
		entity->hitbox.mask |= COLLISIONLAYER_STATIC;

		entity->hitbox.size = Vec2(8.0f, 8.0f);
		entity->sprite.flip_y = false;
		entity->sprite.offset = Vec2(2.0f, 4.0f);

		entity->sprite.setTexture(game->getTexture("walker"));
		entity->velocity.x = 0.0f;

		entity->health = 1;

		entity->state = STATE_START;
		entity->flags[FLAG_ONFLOOR] = false;

		entity->next_state_tick = 0;

		entity->only_update_when_visible = true;
	}

	static bool checkCollisionFloor(const Game *game, const Entity *entity) {
		Hitbox jump_hitbox;

		jump_hitbox.mask = entity->hitbox.mask;

		jump_hitbox.position = entity->hitbox.position;
		jump_hitbox.position.y += entity->hitbox.size.y - 0.1f;
		jump_hitbox.size = entity->hitbox.size;
		jump_hitbox.size.y = 0.4f;

		return game->checkCollision(jump_hitbox);
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		entity->flags[FLAG_ONFLOOR] = checkCollisionFloor(game, entity);

		switch(entity->state) {
			case STATE_START:
				if(entity->flags[FLAG_ONFLOOR]) {
					entity->state = STATE_FLOOR;
				} else{
					entity->state = STATE_JUMP;
				}
				break;

			case STATE_FLOOR:
				if(game->getCurrentTick() > entity->next_state_tick && entity->flags[FLAG_ONFLOOR]) {
					entity->state = STATE_JUMP;
					entity->velocity.y = jump_velocity;
				}

				break;

			case STATE_JUMP:
				if(entity->flags[FLAG_ONFLOOR]) {
					entity->next_state_tick = game->getCurrentTick() + down_time;
					entity->state = STATE_FLOOR;
				}

				break;
		}

		entity->velocity += gravity * dt * gravity_multiplier;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Jumper_GetHandler(void) {
	using namespace Jumper;
	return EntityHandler(create, update, collision);
}
