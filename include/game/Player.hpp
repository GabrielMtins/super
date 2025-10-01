#ifndef PLAYER_HPP
#define PLAYER_HPP

#define PLAYER_MAX_HEALTH 4

#include "game/CustomEntities.hpp"

namespace Player {
	bool hasKey(const Entity *entity);
	void useKey(Entity *entity);
};

#endif
