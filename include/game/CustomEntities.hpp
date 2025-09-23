#ifndef CUSTOMENTITIES_HPP
#define CUSTOMENTITIES_HPP

#include "engine/Game.hpp"
#include "engine/Entity.hpp"
#include "engine/EntityHandler.hpp"

enum EntityHandlerTypes {
	ENTITY_PLAYER,
	ENTITY_ZOMBIE,
	NUM_ENTITY_TYPE
};

enum CollisionLayers {
	COLLISIONLAYER_STATIC = 1 << 0,
	COLLISIONLAYER_PLAYER = 1 << 1,
};

void Custom_AddEntityTypes(Game *game);

EntityHandler Player_GetHandler(void);

EntityHandler Zombie_GetHandler(void);

#endif
