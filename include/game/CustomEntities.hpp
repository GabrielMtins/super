#ifndef CUSTOMENTITIES_HPP
#define CUSTOMENTITIES_HPP

#include "engine/Game.hpp"
#include "engine/Entity.hpp"
#include "engine/EntityHandler.hpp"

enum EntityHandlerTypes {
	ENTITY_PLAYER,
 	ENTITY_WALKER,
 	ENTITY_THROWN,
 	ENTITY_PLATFORM,
 	ENTITY_CAMERA,
 	ENTITY_LIFEBAR,
	NUM_ENTITY_TYPE
};

enum CollisionLayers {
	COLLISIONLAYER_STATIC = 1 << 0,
	COLLISIONLAYER_PLAYER = 1 << 1,
	COLLISIONLAYER_ENEMY = 1 << 2,
	COLLISIONLAYER_THROWABLE = 1 << 3,
};

void Custom_AddEntityTypes(Game *game);

EntityHandler Player_GetHandler(void);

EntityHandler Walker_GetHandler(void);

EntityHandler Thrown_GetHandler(void);

EntityHandler Platform_GetHandler(void);

EntityHandler Camera_GetHandler(void);

EntityHandler Lifebar_GetHandler(void);

#endif
