#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Vec2.hpp"
#include "core/Texture.hpp"

#include "engine/World.hpp"

class Game;

using EntityId = int;
using EntityType = int;

struct Entity {
	enum Axis {
		AXIS_X,
		AXIS_Y
	};

	Entity(void);
	void render(Game *game) const;
	bool checkCollision(const Vec2& other_pos, const Vec2& other_size) const;
	bool checkCollision(const Entity& other) const;
	bool solveCollision(const Entity& other, Axis axis);
	bool checkCollision(const World* world) const;
	bool solveCollision(const World* world, Axis axis);

	Vec2 position;
	Vec2 size;
	Vec2 texture_offset;
	Vec2 velocity;

	EntityId id;
	EntityType type;

	bool alive;

	Texture *texture;
	int texture_cell;
	bool flip_x;
	bool flip_y;

	EntityId target_id;
	EntityId child_id;
	EntityId parent_id;

	uint32_t collision_layer;
	uint32_t collision_mask;
	uint32_t collision_trigger;
};

#endif
