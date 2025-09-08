#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Vec2.hpp"
#include "core/Texture.hpp"

#include "engine/World.hpp"

#include <array>

#define ENTITY_MAX_TIMERS 4
#define ENTITY_MAX_FLAGS 4

class Game;

using EntityId = int;
using EntityType = int;

struct Sprite {
	Sprite(void);
	void setTexture(Texture *texture);
	bool isOnCamera(const Game *game) const;
	void render(Game *game) const;

	Vec2 position;
	Vec2 size;
	Vec2 offset;
	int cell;
	Texture *texture;
	bool hud_element;

	bool flip_x;
	bool flip_y;

	int layer;
};

struct Entity {
	enum Axis {
		AXIS_X,
		AXIS_Y
	};

	Entity(void);
	Entity(EntityId id);
	EntityId getId(void) const;
	void updateSprite(void);
	bool checkCollision(const Vec2& other_pos, const Vec2& other_size) const;
	bool checkCollision(const Entity& other) const;
	bool solveCollision(const Entity& other, Axis axis);
	bool checkCollision(const World* world) const;
	bool solveCollision(const World* world, Axis axis);

	bool solveCollision(const Entity& other);

	Vec2 position;
	Vec2 size;
	Vec2 velocity;

	Sprite sprite;

	EntityType type;

	bool alive;
	bool hud_element;

	EntityId target_id;
	EntityId child_id;
	EntityId parent_id;
	
	int state;

	uint32_t collision_layer;
	uint32_t collision_mask;
	uint32_t collision_trigger;

	std::array<Tick, ENTITY_MAX_TIMERS> timers;
	std::array<int, ENTITY_MAX_FLAGS> flags;

	private:
		EntityId id;
};

#endif
