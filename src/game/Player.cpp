#include "game/CustomEntities.hpp"

namespace Player {
	static void create(Game *game, Entity *entity) {
		entity->texture = game->getTexture("character");
		entity->collision_mask = 1;
		entity->size = Vec2(24.0f, 24.0f);
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) dt;
		entity->velocity = Vec2(0.0f, 0.0f);

		if(game->getKey(Game::INPUT_LEFT)) {
			entity->velocity.x += -40.0f;
		}

		if(game->getKey(Game::INPUT_RIGHT)) {
			entity->velocity.x += 40.0f;
		}

		if(game->getKey(Game::INPUT_DOWN)) {
			entity->velocity.y += 40.0f;
		}

		if(game->getKey(Game::INPUT_UP)) {
			entity->velocity.y += -40.0f;
		}
	}
};

EntityHandler Player_GetHandler(void) {
	return EntityHandler(Player::create, Player::update);
}
