#include "game/CustomEntities.hpp"

void Custom_AddEntityTypes(Game *game) {
	game->addHandlerToType(ENTITY_PLAYER, Player_GetHandler());
	game->addHandlerToType(ENTITY_WALKER, Walker_GetHandler());
	game->addHandlerToType(ENTITY_THROWN, Thrown_GetHandler());
	game->addHandlerToType(ENTITY_PLATFORM, Platform_GetHandler());
	game->addHandlerToType(ENTITY_CAMERA, Camera_GetHandler());
	game->addHandlerToType(ENTITY_LIFEBAR, Lifebar_GetHandler());
	game->addHandlerToType(ENTITY_BALL, Ball_GetHandler());
}
