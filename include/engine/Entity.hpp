#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Vec2.hpp"
#include "core/Texture.hpp"

using EntityId = int;
using EntityType = int;

struct Entity {
	Entity(void);
	void render(Context *context);

	Vec2 position;
	Vec2 texture_offset;
	Vec2 velocity;

	EntityId id;
	EntityType type;

	bool alive;

	Texture *texture;
	int texture_cell;
	bool flip_x;
	bool flip_y;

	EntityId target;
};

#endif
