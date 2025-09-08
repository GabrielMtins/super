#ifndef CUSTOMENTITIES_HPP
#define CUSTOMENTITIES_HPP

#include "engine/Game.hpp"
#include "engine/Entity.hpp"
#include "engine/EntityHandler.hpp"

enum EntityHandlerTypes {
	ENTITY_PLAYER,
	ENTITY_WALKER,
	NUM_ENTITY_TYPE
};

EntityHandler Player_GetHandler(void);

EntityHandler Walker_GetHandler(void);

#endif
