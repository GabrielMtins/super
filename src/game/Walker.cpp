#include "game/CustomEntities.hpp"
#include "game/Constants.hpp"
#include "game/EnemyAI.hpp"

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
		entity->hitbox.mask |= COLLISIONLAYER_STATIC;
		entity->hitbox.mask |= COLLISIONLAYER_ENEMY;

		entity->hitbox.size = Vec2(8.0f, 8.0f);
		entity->sprite.flip_y = false;
		entity->sprite.offset = Vec2(2.0f, 4.0f);

		entity->sprite.setTexture(game->getTexture("walker"));

		entity->state = STATE_RIGHT;

		entity->animator.setAnimation(walking_animation, 200);
		entity->health = 1;

		entity->only_update_when_visible = true;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		bool check_collision_frame = EnemyAI::checkCollisionThisFrame(game);

		switch(entity->state) {
			case STATE_LEFT:
				entity->velocity.x = -walker_speed;
				entity->sprite.flip_x = true;

				if(check_collision_frame) {
					if(!EnemyAI::canWalk(game, entity)) {
						entity->state = STATE_RIGHT;
					}
				}

				break;

			case STATE_RIGHT:
				entity->velocity.x = walker_speed;
				entity->sprite.flip_x = false;

				if(check_collision_frame) {
					if(!EnemyAI::canWalk(game, entity)) {
						entity->state = STATE_LEFT;
					}
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

