#include "game/CustomEntities.hpp"
#include "game/Constants.hpp"

#include "game/Player.hpp"
#include "game/Thrown.hpp"

#define EPS 0.01f

namespace Player {
	static const float max_speed_walking = 40.0f;
	static const float max_speed_running = 100.0f;
	static const float acceleration = 400.0f;
	static const float air_acceleration = 200.0f;
	static const float friction = 200.0f;
	static const float min_speed = 4.0f;
	static const float jump_velocity = -120.0f;

	static const int start_jump_timer_id = 1;

	static const Tick coyote_time = 100;
	static const Tick jump_time = 250;

	static const int idle_animation[] = {0, -1};
	static const int running_animation[] = {1, 2, -1};
	static const int jumping_animation[] = {3, -1};
	static const int throwing_animation[] = {10, -1};
	static const int death_animation[] = {11, -1};

	static const int item_idle_animation[] = {6, -1};
	static const int item_running_animation[] = {7, 8, -1};
	static const int item_jumping_animation[] = {9, -1};

	static const int picking_animation[] = {4, 5, -1};

	static const Vec2 velocity_knockback = Vec2(40.0f, -150.0f);

	enum PlayerFlags {
		FLAG_RUNNING = 0,
		FLAG_ONGROUND,
		FLAG_CANCELJUMP,
		FLAG_HASKEY,
	};

	enum PlayerTimers {
		TIMER_LAST_ON_GROUND = 0,
		TIMER_START_JUMP,
	};

	enum PlayerChildren {
		CHILD_ENEMY_UNDER = 0,
		CHILD_ITEM_HOLDING,
	};

	enum PlayerStates {
		STATE_MOVEMENT,
		STATE_PICKING_ITEM,
		STATE_HOLDING_ITEM,
		STATE_THROWING_ITEM,
		STATE_KNOCKBACK,
		STATE_DEAD
	};

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->sprite.setTexture(game->getTexture("character"));
		entity->sprite.layer = RENDERLAYER_ENTITIES;

		entity->hitbox.size = Vec2(8.0f, 16.0f);
		entity->sprite.offset = Vec2(4.0f, 0.0f);

		entity->hitbox.layer |= COLLISIONLAYER_PLAYER;
		entity->hitbox.mask |= COLLISIONLAYER_STATIC;
		entity->hitbox.mask |= COLLISIONLAYER_ENEMY;
		entity->hitbox.mask |= COLLISIONLAYER_THROWABLE;

		//entity->hitbox.position.x += 128.0f;
		entity->hitbox.position.y -= 128.0f;

		entity->state = STATE_MOVEMENT;
		entity->blink_when_damaged = true;

		entity->direction.x = 1.0f;

		entity->flags[FLAG_HASKEY] = false;

		entity->health = PLAYER_MAX_HEALTH;
		entity->damage_cooldown = 1500;
	}

	static bool isChildUnderPlayer(Entity *entity, Entity *other) {
		float tolerance = other->hitbox.size.y * 0.5f;
		float player_bottom_y = entity->hitbox.position.y + entity->hitbox.size.y;
		float other_top_y = other->hitbox.position.y;

		if(other_top_y > player_bottom_y - tolerance) {
			return true;
		}

		return false;
	}

	static bool isOnGround(const Game *game, const Entity *entity) {
		Hitbox jump_hitbox;

		jump_hitbox.mask = entity->hitbox.mask;

		jump_hitbox.position = entity->hitbox.position;
		jump_hitbox.position.y += entity->hitbox.size.y - 0.1f;
		jump_hitbox.size = entity->hitbox.size;
		jump_hitbox.size.y = 0.4f;

		return game->checkCollision(jump_hitbox);
	}

	static void updateTimers(Game *game, Entity *entity) {
		Tick& last_on_ground_timer = entity->timers[TIMER_LAST_ON_GROUND];

		if(entity->flags[FLAG_ONGROUND]) {
			last_on_ground_timer = game->getCurrentTick();
		}
	}

	static void updateFlags(Game *game, Entity *entity) {
		entity->flags[FLAG_ONGROUND] = isOnGround(game, entity);
		entity->flags[FLAG_RUNNING] = game->getInput(InputType::FIRE);

		if(entity->flags[FLAG_ONGROUND]) {
			entity->flags[FLAG_CANCELJUMP] = false;
		}
	}

	static bool checkJumpCondition(Game *game, Entity *entity) {
		Tick& last_on_ground_timer = entity->timers[TIMER_LAST_ON_GROUND];
		Tick& start_jumper_timer = entity->timers[TIMER_START_JUMP];

		if(entity->velocity.y >= 0.0f) {
			return game->getCurrentTick() < last_on_ground_timer + coyote_time;
		}

		if(entity->flags[FLAG_CANCELJUMP])
			return false;

		return game->getCurrentTick() < start_jumper_timer + jump_time;
	}

	static void applyPhysics(Game *game, Entity *entity, float dt, const Vec2& wish_dir) {
		bool is_on_ground = entity->flags[FLAG_ONGROUND];
		bool has_movement_x = fabsf(wish_dir.x) > EPS;

		entity->velocity += gravity * dt;

		if(!has_movement_x && is_on_ground) {
			if(fabsf(entity->velocity.x) < min_speed) {
				entity->velocity.x = 0.0f;
			} else {
				entity->velocity.x -= copysignf(friction * dt, entity->velocity.x);
			}
		}

		if(has_movement_x) {
			bool is_running = entity->flags[FLAG_RUNNING];
			float max_speed = is_running ? max_speed_running : max_speed_walking;
			bool below_max_speed = fabsf(entity->velocity.x) < max_speed;
			bool move_opposite = wish_dir.x * entity->velocity.x <= 0.0f;

			if(move_opposite || below_max_speed) {
				entity->velocity.x += wish_dir.x * (is_on_ground ? acceleration : air_acceleration) * dt;
				//printf("%f\n", entity->velocity.x);
			}

			if(!below_max_speed && is_on_ground) {
				entity->velocity.x -= copysign(friction * dt, entity->velocity.x);
			}
		}

		if(is_on_ground && fabsf(entity->velocity.x) > 0.0f) {
			entity->direction.x = entity->velocity.x;
		}

		if(fabsf(wish_dir.y) > EPS) {
			if(checkJumpCondition(game, entity)) {
				if(entity->velocity.y >= 0.0f) {
					entity->timers[TIMER_START_JUMP] = game->getCurrentTick();
				}

				entity->velocity.y = jump_velocity;
			}
		}
	}

	static void handleFire(Game *game, Entity *entity) {
		Entity *item;

		switch(entity->state) {
			case STATE_MOVEMENT:
				if(entity->children[CHILD_ENEMY_UNDER]) {
					Entity *child = game->getEntityFromId(entity->children[CHILD_ENEMY_UNDER]);

					entity->state = STATE_PICKING_ITEM;
					entity->next_state_tick = game->getCurrentTick() + 400;

					game->transformEntityToType(child->getId(), ENTITY_THROWN);

					entity->hitbox.size.y += child->hitbox.size.y;
					entity->hitbox.position.y -= child->hitbox.size.y;
					entity->sprite.offset.y -= child->hitbox.size.y;

					entity->children[CHILD_ITEM_HOLDING] = child->getId();
				}
				break;

			case STATE_HOLDING_ITEM:
				entity->state = STATE_THROWING_ITEM;
				entity->next_state_tick = game->getCurrentTick() + 200;

				item = game->getEntityFromId(entity->children[CHILD_ITEM_HOLDING]);

				Thrown::throwEntity(item, entity->velocity * Vec2(0.8f, 1.00f), entity->direction.x);

				if(item) {
					entity->hitbox.size.y -= item->hitbox.size.y;
					entity->hitbox.position.y += item->hitbox.size.y;
					entity->sprite.offset.y += item->hitbox.size.y;
				}
				break;
		}
	}

	static Vec2 handleInput(Game *game, Entity *entity) {
		Vec2 wish_dir;

		if(game->getInput(InputType::JUMP) && !entity->flags[FLAG_CANCELJUMP]) {
			wish_dir.y = -1.0f;
		}
		if(game->getInputUp(InputType::JUMP) && entity->velocity.y < 0.0f) {
			entity->flags[FLAG_CANCELJUMP] = true;
		}

		if(game->getInput(InputType::LEFT)) {
			wish_dir.x -= 1.0f;
		}

		if(game->getInput(InputType::RIGHT)) {
			wish_dir.x += 1.0f;
		}

		if(game->getInputDown(InputType::FIRE)) {
			handleFire(game, entity);
		}

		return wish_dir;
	}

	static void handleAnimationsDefault(Entity *entity) {
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

	static void updateItemHolding(Game *game, Entity *entity) {
		Entity *item_holding;

		item_holding = game->getEntityFromId(entity->children[CHILD_ITEM_HOLDING]);

		item_holding->sprite.flip_x = entity->direction.x < 0.0f;
		item_holding->sprite.flip_y = true;

		item_holding->hitbox.position = Vec2(
				entity->hitbox.position.x,
				entity->hitbox.position.y - item_holding->hitbox.size.y - entity->sprite.offset.y
				);
	}

	static void updateItemPicking(Game *game, Entity *entity) {
		Entity *item_holding;

		item_holding = game->getEntityFromId(entity->children[CHILD_ITEM_HOLDING]);

		item_holding->sprite.flip_x = entity->direction.x < 0.0f;
		item_holding->sprite.flip_y = true;

		item_holding->hitbox.position = Vec2(
				entity->hitbox.position.x,
				entity->hitbox.position.y + entity->hitbox.size.y
				);

		if(game->getCurrentTick() < entity->next_state_tick - 300) {
			item_holding->hitbox.position.y -= 0;
		} else if(game->getCurrentTick() < entity->next_state_tick - 200) {
			item_holding->hitbox.position.y -= 8;
		} else if(game->getCurrentTick() < entity->next_state_tick) {
			item_holding->hitbox.position.y -= 20;
		}
	}

	static void handleAnimationsItem(Entity *entity) {
		entity->sprite.flip_x = entity->direction.x < 0.0f;

		if(fabsf(entity->velocity.x) > 0.01f) {
			entity->animator.setAnimation(item_running_animation, 100);
		} else {
			entity->animator.setAnimation(item_idle_animation, 100);
		}

		if(fabsf(entity->velocity.y) > 0.00f) {
			entity->animator.setAnimation(item_jumping_animation, 100);
		}
	}

	static void applyKnockback(Game *game, Entity *entity, Entity *other) {
		if(entity->state == STATE_KNOCKBACK)
			return;

		if(entity->state == STATE_HOLDING_ITEM || entity->state == STATE_PICKING_ITEM) {
			handleFire(game, entity);
		}

		entity->state = STATE_KNOCKBACK;
		entity->next_state_tick = game->getCurrentTick() + 400;
		entity->velocity = velocity_knockback;

		if(other != NULL)
			entity->velocity.x = copysignf(entity->velocity.x, entity->center.x - other->center.x);
	}

	static void damageHandler(Game *game, Entity *entity, Entity *other) {
		if(entity->getDamage(game, 1)) {
			applyKnockback(game, entity, other);
		} 

		if(entity->health == 1) {
			entity->hitbox.mask = 0;
			entity->hitbox.layer = 0;
			entity->state = STATE_DEAD;
			entity->sprite.layer = RENDERLAYER_THROWN;
			entity->blink_when_damaged = false;
			entity->velocity.x = 0.0f;
			entity->velocity.y = -200.0f;
		}
	}

	static void handleItemCollision(Game *game, Entity *entity) {
		Entity *other;
		Hitbox hitbox = entity->hitbox;
		hitbox.mask = COLLISIONLAYER_ITEMS;

		if((game->getCurrentFrame() & 0x3))
			return;

		EntityFoundList list = game->findCollision(hitbox);

		for(const EntityId& id : list) {
			other = game->getEntityFromId(id);

			switch(other->type) {
				case ENTITY_KEYITEM:
					entity->flags[FLAG_HASKEY] = true;
					other->alive = false;
					break;

				case ENTITY_HEARTITEM:
					if(entity->health < PLAYER_MAX_HEALTH) {
						entity->health++;
					}

					other->alive = false;
					break;
			}
		}
	}

	static void handleEnemyNoTouch(Game *game, Entity *entity) {
		Entity *other;
		Hitbox hitbox = entity->hitbox;
		hitbox.mask = COLLISIONLAYER_ENEMY_NOTOUCH;

		if((game->getCurrentFrame() & 0x1))
			return;

		EntityFoundList list = game->findCollision(hitbox);

		for(const EntityId& id : list) {
			other = game->getEntityFromId(id);

			switch(other->type) {
				case ENTITY_ENEMYBULLET:
					damageHandler(game, entity, other);
					other->alive = false;
					break;
			}
		}
	}

	static void update(Game *game, Entity *entity, float dt) {
		updateFlags(game, entity);
		updateTimers(game, entity);
		handleItemCollision(game, entity);
		handleEnemyNoTouch(game, entity);

		switch(entity->state) {
			case STATE_MOVEMENT:
				handleAnimationsDefault(entity);
				applyPhysics(
						game,
						entity,
						dt,
						handleInput(game, entity)
						);
				break;

			case STATE_HOLDING_ITEM:
				handleAnimationsItem(entity);
				applyPhysics(
						game,
						entity,
						dt,
						handleInput(game, entity)
						);
				updateItemHolding(game, entity);
				break;

			case STATE_THROWING_ITEM:
				entity->animator.setAnimation(throwing_animation, 200);

				applyPhysics(
						game,
						entity,
						dt,
						handleInput(game, entity)
						);

				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_MOVEMENT;
				}
				break;

			case STATE_PICKING_ITEM:
				entity->animator.setAnimation(picking_animation, 200);
				entity->velocity = Vec2::zero;
				updateItemPicking(game, entity);

				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_HOLDING_ITEM;
				}
				break;

			case STATE_KNOCKBACK:
				entity->animator.setAnimation(jumping_animation, 200);

				applyPhysics(
						game,
						entity,
						dt,
						Vec2::zero
						);

				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_MOVEMENT;
				}
				break;

			case STATE_DEAD:
				entity->animator.setAnimation(death_animation, 200);

				applyPhysics(
						game,
						entity,
						dt,
						Vec2::zero
						);
				break;
		}

		if(entity->children[CHILD_ENEMY_UNDER]) {
			Entity *child = game->getEntityFromId(entity->children[CHILD_ENEMY_UNDER]);

			entity->contact_velocity = child->velocity;
		} else {
			entity->contact_velocity = Vec2::zero;
		}

		// printf("%f\n", 1.0f / dt);

		entity->children[CHILD_ENEMY_UNDER] = 0;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
		
		bool is_child_under_player;

		if(other == NULL)
			return;

		is_child_under_player = isChildUnderPlayer(entity, other);

		if(is_child_under_player && (other->hitbox.layer & COLLISIONLAYER_THROWABLE)) {
			entity->children[CHILD_ENEMY_UNDER] = other->getId();
		}

		if(!is_child_under_player && (other->hitbox.layer & COLLISIONLAYER_ENEMY)) {
			damageHandler(game, entity, other);
		}
	}

	bool hasKey(const Entity *entity) {
		return entity->flags[FLAG_HASKEY];
	}

	void useKey(Entity *entity) {
		entity->flags[FLAG_HASKEY] = false;
	}
}

EntityHandler Player_GetHandler(void) {
	using namespace Player;
	return EntityHandler(create, update, collision);
}
