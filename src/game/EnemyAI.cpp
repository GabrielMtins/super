#include "game/EnemyAI.hpp"

namespace EnemyAI {
	bool aboutToCollide(const Game *game, const Entity *entity) {
		Hitbox hitbox;

		hitbox = entity->hitbox;

		if(entity->velocity.x > 0.0f) {
			hitbox.position.x += hitbox.size.x + 0.1f;
		} else {
			hitbox.position.x -= 1.1f;
		}

		hitbox.size.x = 1.0f;
		hitbox.size.y /= 2;

		hitbox.mask = (COLLISIONLAYER_ENEMY | COLLISIONLAYER_STATIC);

		return game->checkCollision(hitbox);
	}

	bool willFall(const Game *game, const Entity *entity) {
		Hitbox hitbox;

		hitbox = entity->hitbox;

		if(entity->velocity.x > 0.0f) {
			hitbox.position.x += hitbox.size.x + 0.1f;
		} else {
			hitbox.position.x -= hitbox.size.x + 0.1f;
		}

		hitbox.position.y += hitbox.size.y + 0.1f;

		hitbox.size.y = 1.0f;

		hitbox.mask = (COLLISIONLAYER_STATIC);

		return !game->checkCollision(hitbox);
	}

	bool canWalk(const Game *game, const Entity *entity) {
		if(aboutToCollide(game, entity))
			return false;

		if(willFall(game, entity))
			return false;

		return true;
	}

	bool checkCollisionThisFrame(const Game *game) {
		return !(game->getCurrentFrame() & 0xf);
	}
};
