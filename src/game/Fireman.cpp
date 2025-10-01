#include "game/CustomEntities.hpp"
#include "game/EnemyAI.hpp"
#include "game/EnemyBullet.hpp"

namespace Fireman {
	enum FiremanStates {
		STATE_LEFT,
		STATE_RIGHT,
		STATE_FIRING_LEFT,
		STATE_FIRING_RIGHT,
		STATE_IDLE_LEFT,
		STATE_IDLE_RIGHT,
	};

	static const int idle_animation[] = {0, -1};
	static const int walking_animation[] = {0, 1, -1};
	static const float speed = 20.0f;
	static const float bullet_speed = 100.0f;

	static const Tick walk_time = 3000;
	static const Tick idle_time = 500;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_FIREMAN;

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
		entity->next_state_tick = game->getCurrentTick() + walk_time;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		bool check_collision_frame = !(game->getCurrentFrame() & 0xf);

		switch(entity->state) {
			case STATE_LEFT:
				entity->animator.setAnimation(walking_animation, 200);
				entity->velocity.x = -speed;
				entity->sprite.flip_x = true;

				if(check_collision_frame) {
					if(!EnemyAI::canWalk(game, entity)) {
						entity->state = STATE_RIGHT;
					}
				}

				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_FIRING_LEFT;
				}

				break;

			case STATE_RIGHT:
				entity->animator.setAnimation(walking_animation, 200);
				entity->velocity.x = speed;
				entity->sprite.flip_x = false;

				if(check_collision_frame) {
					if(!EnemyAI::canWalk(game, entity)) {
						entity->state = STATE_LEFT;
					}
				}

				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_FIRING_RIGHT;
				}

				break;

			case STATE_FIRING_LEFT:
				entity->state = STATE_IDLE_LEFT;
				entity->next_state_tick = game->getCurrentTick() + idle_time;
				EnemyBullet::add(game, entity->hitbox.position - Vec2(5.0f, 0.0f), Vec2(-bullet_speed, 0.0f));
				break;

			case STATE_FIRING_RIGHT:
				entity->state = STATE_IDLE_RIGHT;
				entity->next_state_tick = game->getCurrentTick() + idle_time;
				EnemyBullet::add(game, entity->hitbox.position + Vec2(8.0f, 0.0f), Vec2(bullet_speed, 0.0f));
				break;

			case STATE_IDLE_LEFT:
				entity->animator.setAnimation(idle_animation, 200);
				entity->velocity.x = 0.0f;
				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_LEFT;
					entity->next_state_tick = game->getCurrentTick() + walk_time;
				}

				break;

			case STATE_IDLE_RIGHT:
				entity->animator.setAnimation(idle_animation, 200);
				entity->velocity.x = 0.0f;
				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_RIGHT;
					entity->next_state_tick = game->getCurrentTick() + walk_time;
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

EntityHandler Fireman_GetHandler(void) {
	using namespace Fireman;
	return EntityHandler(create, update, collision);
}
