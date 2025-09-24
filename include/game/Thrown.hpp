#ifndef THROWN_HPP
#define THROWN_HPP

#include "game/CustomEntities.hpp"

void Thrown_CopyEntity(Entity *entity, const Entity *other);

void Thrown_Throw(Entity *entity, float speed, float direction);

#endif
