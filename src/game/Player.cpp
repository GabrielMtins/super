#include "game/CustomEntities.hpp"
#include "game/Constants.hpp"

#define EPS 0.01f

namespace Player {
	static const float max_speed_walking = 40.0f;
	static const float max_speed_running = 120.0f;
	static const float acceleration = 500.0f;
	static const float air_acceleration = 250.0f;
	static const float friction = 200.0f;
	static const float min_speed = 10.0f;
	static const float jump_velocity = -150.0f;

	static const int last_on_ground_timer_id = 0;
	static const int start_jump_timer_id = 1;

	static const Tick coyote_time = 100;
	static const Tick jump_time = 250;

	static const int idle_animation[] = {0, -1};
	static const int running_animation[] = {1, 2, -1};
	static const int jumping_animation[] = {3, -1};

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->sprite.setTexture(game->getTexture("character"));

		entity->hitbox.size = Vec2(8.0f, 16.0f);
		entity->sprite.offset = Vec2(4.0f, 0.0f);

		entity->hitbox.layer |= COLLISIONLAYER_PLAYER;
		entity->hitbox.mask |= COLLISIONLAYER_STATIC;
		entity->damage_cooldown = 200;
	}

	static bool isOnGround(Game *game, Entity *entity) {
		Hitbox jump_hitbox;

		jump_hitbox.mask |= COLLISIONLAYER_STATIC;

		jump_hitbox.position = entity->hitbox.position;
		jump_hitbox.position.y += entity->hitbox.size.y - 0.1f;
		jump_hitbox.size = entity->hitbox.size;
		jump_hitbox.size.y = 0.4f;

		return game->checkCollision(jump_hitbox);
	}

	static void updateTimers(Game *game, Entity *entity) {
		Tick& last_on_ground_timer = entity->timers[last_on_ground_timer_id];

		if(isOnGround(game, entity)) {
			last_on_ground_timer = game->getCurrentTick();
		}
	}

	static bool checkJumpCondition(Game *game, Entity *entity) {
		Tick& last_on_ground_timer = entity->timers[last_on_ground_timer_id];
		Tick& start_jumper_timer = entity->timers[start_jump_timer_id];

		if(entity->velocity.y >= 0.0f) {
			return game->getCurrentTick() < last_on_ground_timer + coyote_time;
		}

		return game->getCurrentTick() < start_jumper_timer + jump_time;
	}

	static void applyPhysics(Game *game, Entity *entity, float dt, const Vec2& wish_dir) {
		bool is_on_ground = isOnGround(game, entity);
		entity->velocity += gravity * dt;

		if(fabsf(wish_dir.x) < EPS && is_on_ground) {
			if(fabsf(entity->velocity.x) < min_speed) {
				entity->velocity.x = 0.0f;
			} else {
				entity->velocity.x -= copysignf(friction * dt, entity->velocity.x);
			}
		}

		if(fabsf(wish_dir.x) > EPS) {
			bool is_running = game->getKey(Game::INPUT_FIRE);
			float max_speed = is_running ? max_speed_running : max_speed_walking;
			bool below_max_speed = fabsf(entity->velocity.x) < max_speed;

			if(wish_dir.x * entity->velocity.x <= 0.0f || below_max_speed) {
				entity->velocity.x += wish_dir.x * (is_on_ground ? acceleration : air_acceleration) * dt;
			}

			if(!below_max_speed && is_on_ground) {
				entity->velocity.x -= copysign(friction * dt, entity->velocity.x);
			}

			if(is_on_ground)
				entity->direction.x = wish_dir.x;
		}

		if(fabsf(wish_dir.y) > EPS) {
			if(checkJumpCondition(game, entity)) {
				if(entity->velocity.y >= 0.0f) {
					entity->timers[start_jump_timer_id] = game->getCurrentTick();
				}

				entity->velocity.y = jump_velocity;
			}
		}
	}

	static Vec2 handleInput(Game *game, Entity *entity) {
		Vec2 wish_dir;

		if(game->getKey(Game::INPUT_JUMP)) {
			wish_dir.y = -1.0f;
		}

		if(game->getKey(Game::INPUT_LEFT)) {
			wish_dir.x -= 1.0f;
		}

		if(game->getKey(Game::INPUT_RIGHT)) {
			wish_dir.x += 1.0f;
		}

		return wish_dir;
	}

	static void handleAnimations(Entity *entity) {
		entity->sprite.flip_x = entity->direction.x < 0.0f;

		if(fabsf(entity->velocity.x) > 0.01f) {
			entity->animator.setAnimation(running_animation, 100);
		} else {
			entity->animator.setAnimation(idle_animation, 100);
		}

		if(fabsf(entity->velocity.y) > 0.00f) {
			entity->animator.setAnimation(jumping_animation, 100);
		}
	}

	static void update(Game *game, Entity *entity, float dt) {
		handleAnimations(entity);
		updateTimers(game, entity);
		applyPhysics(
				game,
				entity,
				dt,
				handleInput(game, entity)
				);

		game->setCameraPosition(
				Vec2(
					entity->hitbox.position.x - 100,
					-16.0f
					)
				);
		/*
		game->setCameraPosition(
				entity->hitbox.position - game->getScreenDimensions() * 0.5f
				);
				*/
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Player_GetHandler(void) {
	using namespace Player;
	return EntityHandler(create, update, collision);
}
