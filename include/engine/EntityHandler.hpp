#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP

#include "engine/Entity.hpp"

class Game;

using EntityCreateCallback = void (*)(Game *, Entity *);
using EntityUpdateCallback = void (*)(Game *, Entity *, float);
using EntityCollisionCallback = void (*) (Game *, Entity *, Entity *);

struct EntityHandler {
	EntityHandler(void);
	EntityHandler(EntityCreateCallback create, EntityUpdateCallback update, EntityCollisionCallback collision);

	EntityCreateCallback create;
	EntityUpdateCallback update;
	EntityCollisionCallback collision;
};

#endif
