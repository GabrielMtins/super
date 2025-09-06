#include "game/CustomEntities.hpp"

static void Player_Create(Game *game, Entity *entity) {
	entity->texture = game->getTexture("character");
}

static void Player_Update(Game *game, Entity *entity, float dt) {
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

	entity->position += entity->velocity * dt;
}

EntityHandler Player_GetHandler(void) {
	return EntityHandler(Player_Create, Player_Update);
}
