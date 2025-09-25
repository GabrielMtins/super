#ifndef THROWN_HPP
#define THROWN_HPP

#include "game/CustomEntities.hpp"

namespace Thrown {
	void copyEntity(Entity *entity, const Entity *other);
	void throwEntity(Entity *entity, const Vec2& velocity, float direction);
};

#endif
