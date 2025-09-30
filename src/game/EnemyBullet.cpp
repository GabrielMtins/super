#include "game/CustomEntities.hpp"
#include "game/EnemyBullet.hpp"

namespace EnemyBullet {
	enum EnemyBulletStates {
		STATE_ALIVE,
		STATE_DEAD
	};

	static const Tick time_to_live = 5000;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_ENEMYBULLET;

		entity->sprite.setTexture(game->getTexture("8x8-items"));
		entity->sprite.cell = 3;

		entity->hitbox.size = Vec2(4.0f, 4.0f);
		entity->hitbox.layer = COLLISIONLAYER_ENEMY_NOTOUCH;
		entity->hitbox.mask = COLLISIONLAYER_ENEMY | COLLISIONLAYER_STATIC;

		entity->state = STATE_ALIVE;
		entity->next_state_tick = game->getCurrentTick() + time_to_live;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		switch(entity->state) {
			case STATE_ALIVE:
				if(game->getCurrentTick() > entity->next_state_tick) {
					entity->state = STATE_DEAD;
				}
				break;

			case STATE_DEAD:
				entity->alive = false;
				break;
		}
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;

		entity->alive = false;
	}

	void add(Game *game, const Vec2& position, const Vec2& velocity) {
		Entity *entity = game->getEntityFromId(game->addEntity(ENTITY_ENEMYBULLET));

		entity->hitbox.position = position;
		entity->velocity = velocity;
	}
}

EntityHandler EnemyBullet_GetHandler(void) {
	using namespace EnemyBullet;
	return EntityHandler(create, update, collision);
}
