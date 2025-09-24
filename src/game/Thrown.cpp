#include "game/CustomEntities.hpp"
#include "game/Constants.hpp"
#include "game/Thrown.hpp"

namespace Thrown {
	enum ThrownFlags {
		FLAG_CARRIED = 0,
	};

	enum ThrownCounters {
		COUNTER_TYPE = 0,
		COUNTER_BOUNCE
	};

	static const float speed = 100.0f;
	static const float jump = -100.0f;

	static const Vec2 hit_velocity = Vec2(20.0f, -150.0f);

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_THROWN;

		entity->hitbox.mask |= COLLISIONLAYER_ENEMY_THROWABLE;
		entity->flags[Thrown::FLAG_CARRIED] = true;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		if(entity->flags[FLAG_CARRIED])
			return;

		entity->velocity += gravity * dt;

		if(entity->counters[COUNTER_BOUNCE] >= 2 && entity->velocity.y < 0.0f) {
			entity->alive = false;

			Entity *actual = game->getEntityFromId(game->addEntity(entity->counters[COUNTER_TYPE]));
			actual->hitbox.position = entity->hitbox.position;
		}
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;

		if(other == NULL && entity->velocity.y == 0.0f) {
			entity->velocity.y = jump;
			entity->counters[COUNTER_BOUNCE]++;
		}

		if(other != NULL) {
			Entity *new_throw = game->getEntityFromId(game->addEntity(ENTITY_THROWN));
			other->alive = false;

			Thrown_CopyEntity(new_throw, other);
			new_throw->hitbox.mask = 0;
			new_throw->flags[FLAG_CARRIED] = false;
			new_throw->velocity = hit_velocity;
			entity->velocity.y = jump;
			entity->hitbox.mask = 0;
		}
	}
}

EntityHandler Thrown_GetHandler(void) {
	using namespace Thrown;
	return EntityHandler(create, update, collision);
}

void Thrown_CopyEntity(Entity *entity, const Entity *other) {
	entity->hitbox = other->hitbox;

	entity->hitbox.mask = COLLISIONLAYER_ENEMY_THROWABLE;
	entity->hitbox.layer = 0;

	entity->sprite = other->sprite;
	entity->sprite.flip_y = true;
	entity->sprite.hud_element = true;

	entity->sprite.offset.y = 0.0f;

	entity->counters[Thrown::COUNTER_TYPE] = other->type;
}

void Thrown_Throw(Entity *entity, const Vec2& velocity, float direction) {
	entity->velocity = Vec2(copysignf(Thrown::speed, direction), 0.0f) + velocity;

	entity->hitbox.mask |= COLLISIONLAYER_STATIC;
	entity->flags[Thrown::FLAG_CARRIED] = false;
}
