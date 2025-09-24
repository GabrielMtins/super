#include "game/CustomEntities.hpp"
#include "game/Constants.hpp"

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

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_THROWN;

		entity->hitbox.mask |= COLLISIONLAYER_ENEMY_THROWABLE;
		entity->flags[FLAG_CARRIED] = true;
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

void Thrown_Throw(Entity *entity, float speed, float direction) {
	entity->velocity = Vec2(copysignf(Thrown::speed, direction) + speed, 0.0f);

	entity->hitbox.mask |= COLLISIONLAYER_STATIC;
	entity->flags[Thrown::FLAG_CARRIED] = false;
}
