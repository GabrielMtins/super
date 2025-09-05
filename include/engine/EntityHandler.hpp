#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP

#include "engine/Entity.hpp"

class Game;

using EntityCreateCallback = void (*)(Game *, Entity *);
using EntityUpdateCallback = void (*)(Game *, Entity *, float);

struct EntityHandler {
	EntityHandler(void);
	EntityHandler(EntityCreateCallback create, EntityUpdateCallback update);

	EntityCreateCallback create;
	EntityUpdateCallback update;
};

#endif
