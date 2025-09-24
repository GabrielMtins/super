#include "game/CustomEntities.hpp"
#include "game/Constants.hpp"

namespace Walker {
	enum WalkerStates {
		STATE_LEFT,
		STATE_RIGHT
	};

	static const float walker_speed = 16.0f;

	static const int walking_animation[] = {0, 1, -1};

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_WALKER;

		entity->hitbox.layer |= COLLISIONLAYER_ENEMY_THROWABLE;
		entity->hitbox.mask |= COLLISIONLAYER_STATIC;

		entity->hitbox.size = Vec2(8.0f, 8.0f);
		entity->sprite.offset = Vec2(2.0f, 4.0f);

		entity->sprite.setTexture(game->getTexture("walker"));

		entity->state = STATE_RIGHT;

		entity->animator.setAnimation(walking_animation, 200);
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		switch(entity->state) {
			case STATE_LEFT:
				entity->velocity.x = -walker_speed;
				entity->sprite.flip_x = true;
				break;

			case STATE_RIGHT:
				entity->velocity.x = walker_speed;
				entity->sprite.flip_x = false;
				break;
		}

		entity->velocity += gravity * dt;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;

		if(entity->velocity.x == 0.0f) {
			if(entity->state == STATE_LEFT) {
				entity->state = STATE_RIGHT;
			} else {
				entity->state = STATE_LEFT;
			}
		}
	}
}

EntityHandler Walker_GetHandler(void) {
	using namespace Walker;
	return EntityHandler(create, update, collision);
}

