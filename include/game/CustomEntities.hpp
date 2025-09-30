#ifndef CUSTOMENTITIES_HPP
#define CUSTOMENTITIES_HPP

#include "engine/Game.hpp"
#include "engine/Entity.hpp"
#include "engine/EntityHandler.hpp"
#include "game/Constants.hpp"

enum EntityHandlerTypes {
	ENTITY_PLAYER,
 	ENTITY_WALKER,
 	ENTITY_THROWN,
 	ENTITY_PLATFORM,
 	ENTITY_CAMERA,
 	ENTITY_LIFEBAR,
 	ENTITY_BALL,
 	ENTITY_ITEMBOX,
 	ENTITY_HEARTITEM,
 	ENTITY_JUMPER,
 	ENTITY_FIREMAN,
 	ENTITY_ENEMYBULLET,
	NUM_ENTITY_TYPE
};

enum CollisionLayers {
	COLLISIONLAYER_STATIC = 1 << 0,
	COLLISIONLAYER_PLAYER = 1 << 1,
	COLLISIONLAYER_ENEMY = 1 << 2,
	COLLISIONLAYER_THROWABLE = 1 << 3,
	COLLISIONLAYER_ITEMS = 1 << 4,
	COLLISIONLAYER_ENEMY_NOTOUCH = 1 << 5,
};

void Custom_AddEntityTypes(Game *game);

EntityHandler Player_GetHandler(void);

EntityHandler Walker_GetHandler(void);

EntityHandler Thrown_GetHandler(void);

EntityHandler Platform_GetHandler(void);

EntityHandler Camera_GetHandler(void);

EntityHandler Lifebar_GetHandler(void);

EntityHandler Ball_GetHandler(void);

EntityHandler ItemBox_GetHandler(void);

EntityHandler HeartItem_GetHandler(void);

EntityHandler Jumper_GetHandler(void);

EntityHandler Fireman_GetHandler(void);

EntityHandler EnemyBullet_GetHandler(void);

#endif
