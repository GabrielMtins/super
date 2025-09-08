#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Vec2.hpp"
#include "core/Texture.hpp"

#include "engine/World.hpp"

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
	void updateSprite(void);
	bool checkCollision(const Vec2& other_pos, const Vec2& other_size) const;
	bool checkCollision(const Entity& other) const;
	bool solveCollision(const Entity& other, Axis axis);
	bool checkCollision(const World* world) const;
	bool solveCollision(const World* world, Axis axis);

	Vec2 position;
	Vec2 size;
	Vec2 velocity;

	Sprite sprite;

	EntityId id;
	EntityType type;

	bool alive;
	bool hud_element;

	EntityId target_id;
	EntityId child_id;
	EntityId parent_id;

	uint32_t collision_layer;
	uint32_t collision_mask;
	uint32_t collision_trigger;
};

#endif
