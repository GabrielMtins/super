#include "game/CustomEntities.hpp"
#include "game/Constants.hpp"
#include "game/Thrown.hpp"

#include <unordered_set>

/*
 * Descrição de comportamento:
 * Quando uma entidade que pode ser jogada (caixa, inimigo etc)
 * é jogada, ela se transforma numa entidade do tipo throw. 
 */

namespace Thrown {
	enum ThrownFlags {
		FLAG_CARRIED = 0,
		FLAG_GOINGDOWN,
	};

	enum ThrownCounters {
		COUNTER_BOUNCE = 0,
	};

	static const float speed = 80.0f;
	static const float jump = -100.0f;

	static const std::unordered_set<EntityType> imortal_types = {
		ENTITY_BALL
	};

	static const Vec2 hit_velocity = Vec2(20.0f, -150.0f);

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_THROWN;

		entity->hitbox.mask |= COLLISIONLAYER_ENEMY;
		entity->flags[FLAG_CARRIED] = true;
		entity->flags[FLAG_GOINGDOWN] = true;
		entity->counters[COUNTER_BOUNCE] = 0;

		entity->hitbox.mask = COLLISIONLAYER_ENEMY;
		entity->hitbox.layer = 0;
	
		entity->sprite.flip_y = true;
		entity->sprite.hud_element = true;
	
		entity->sprite.offset.y = 0.0f;
	}

	static void collisionInteractionBehavior(Game *game, Entity *entity) {
		switch(entity->old_type) {
			case ENTITY_ITEMBOX:
				game->transformEntityToType(entity->getId(), entity->alt_type);
				break;

			default:
				if(entity->counters[COUNTER_BOUNCE] >= 2) {
					game->transformEntityToType(entity->getId(), entity->old_type);
				}
				break;
		}
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		if(entity->flags[FLAG_CARRIED])
			return;

		entity->velocity += gravity * dt;

		if(entity->velocity.y != 0.0f)
			entity->flags[FLAG_GOINGDOWN] = entity->velocity.y > 0.0f;
	}

	static bool isImortal(Entity *entity) {
		return imortal_types.find(entity->old_type) != imortal_types.end();
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;

		bool is_static;

		if(entity->flags[FLAG_CARRIED])
			return;

		is_static = other == NULL;

		if(other != NULL) {
			is_static = is_static || (other->hitbox.layer & COLLISIONLAYER_STATIC);
		}

		if(is_static) {
			if(entity->velocity.y == 0.0f && entity->flags[FLAG_GOINGDOWN]) {
				entity->velocity.y = jump;
				entity->counters[COUNTER_BOUNCE]++;
			}
		}

		if(other != NULL && !is_static) {
			other->getDamage(game, 1);
			
			if(!other->health) {
				game->transformEntityToType(other->getId(), ENTITY_THROWN);
			}

			other->hitbox.mask = 0;
			other->flags[FLAG_CARRIED] = false;
			other->velocity = hit_velocity;
			entity->velocity.y = jump;

			if(!isImortal(entity)) {
				entity->hitbox.mask = COLLISIONLAYER_ENEMY;
			}
		}

		collisionInteractionBehavior(game, entity);
	}

	void copyEntity(Entity *entity, const Entity *other) {
	}
	
	void throwEntity(Entity *entity, const Vec2& velocity, float direction) {
		entity->velocity = Vec2(copysignf(Thrown::speed, direction), 0.0f) + velocity;
	
		entity->hitbox.mask |= COLLISIONLAYER_STATIC;
		entity->hitbox.mask |= COLLISIONLAYER_THROWABLE;
		entity->flags[Thrown::FLAG_CARRIED] = false;
	}
}

EntityHandler Thrown_GetHandler(void) {
	using namespace Thrown;
	return EntityHandler(create, update, collision);
}
