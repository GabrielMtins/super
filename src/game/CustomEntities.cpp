#include "game/CustomEntities.hpp"

void Custom_AddEntityTypes(Game *game) {
	game->addHandlerToType(ENTITY_PLAYER, Player_GetHandler());
	game->addHandlerToType(ENTITY_ZOMBIE, Zombie_GetHandler());
}
