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

		entity->hitbox.layer |= COLLISIONLAYER_ENEMY;
		entity->hitbox.layer |= COLLISIONLAYER_THROWABLE;
		//entity->hitbox.layer |= COLLISIONLAYER_STATIC;
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

		Hitbox hitbox = entity->hitbox;
		hitbox.mask |= COLLISIONLAYER_ENEMY;
		hitbox.size.y /= 2;

		switch(entity->state) {
			case STATE_LEFT:
				hitbox.position.x -= 2.0f;
				hitbox.size.x = 1.0f;
				entity->velocity.x = -walker_speed;
				entity->sprite.flip_x = true;

				if(game->checkCollision(hitbox)) {
					entity->state = STATE_RIGHT;
				}
				break;

			case STATE_RIGHT:
				hitbox.position.x += hitbox.size.x + 1.0f;
				hitbox.size.x = 1.0f;
				entity->velocity.x = walker_speed;
				entity->sprite.flip_x = false;

				if(game->checkCollision(hitbox)) {
					entity->state = STATE_LEFT;
				}

				break;
		}

		entity->velocity += gravity * dt;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Walker_GetHandler(void) {
	using namespace Walker;
	return EntityHandler(create, update, collision);
}

