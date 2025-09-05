#include "game/CustomEntities.hpp"

static void Player_Create(Game *game, Entity *entity) {
	entity->texture = &game->texture;
	entity->velocity = Vec2(40.0f, 20.0f);
}

static void Player_Update(Game *game, Entity *entity, float dt) {
	(void) game;

	entity->position += entity->velocity * dt;
}

EntityHandler Player_GetHandler(void) {
	return EntityHandler(Player_Create, Player_Update);
}
