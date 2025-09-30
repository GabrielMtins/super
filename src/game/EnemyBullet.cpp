#include "game/CustomEntities.hpp"

namespace EnemyBullet {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_ENEMYBULLET;

		entity->sprite.setTexture(game->getTexture("8x8-items"));
		entity->sprite.cell = 3;

		entity->hitbox.size = Vec2(4.0f, 4.0f);
		entity->hitbox.layer = COLLISIONLAYER_ENEMY_NOTOUCH;

		entity->hitbox.position.y = 40.0f;
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler EnemyBullet_GetHandler(void) {
	using namespace EnemyBullet;
	return EntityHandler(create, update, collision);
}
