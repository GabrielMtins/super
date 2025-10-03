#include "game/CustomEntities.hpp"

void Custom_AddEntityTypes(Game *game) {
	game->addHandlerToType(ENTITY_PLAYER, Player_GetHandler());
	game->addHandlerToType(ENTITY_WALKER, Walker_GetHandler());
	game->addHandlerToType(ENTITY_THROWN, Thrown_GetHandler());
	game->addHandlerToType(ENTITY_PLATFORM, Platform_GetHandler());
	game->addHandlerToType(ENTITY_CAMERA, Camera_GetHandler());
	game->addHandlerToType(ENTITY_BALL, Ball_GetHandler());
	game->addHandlerToType(ENTITY_ITEMBOX, ItemBox_GetHandler());
	game->addHandlerToType(ENTITY_HEARTITEM, HeartItem_GetHandler());
	game->addHandlerToType(ENTITY_JUMPER, Jumper_GetHandler());
	game->addHandlerToType(ENTITY_FIREMAN, Fireman_GetHandler());
	game->addHandlerToType(ENTITY_ENEMYBULLET, EnemyBullet_GetHandler());
	game->addHandlerToType(ENTITY_HUD, Hud_GetHandler());
	game->addHandlerToType(ENTITY_KEYITEM, KeyItem_GetHandler());
	game->addHandlerToType(ENTITY_DOOR, Door_GetHandler());
	game->addHandlerToType(ENTITY_BACKGROUND, Background_GetHandler());
	game->addHandlerToType(ENTITY_MENUHANDLER, MenuHandler_GetHandler());
}
