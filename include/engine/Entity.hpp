#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Vec2.hpp"
#include "core/Texture.hpp"

#include "engine/EngineDef.hpp"
#include "engine/Sprite.hpp"
#include "engine/Animator.hpp"

#include <array>

class Game;
class World;

struct Entity {
	enum Axis {
		AXIS_X,
		AXIS_Y
	};

	enum PauseMode {
		PAUSEMODE_PAUSABLE,
		PAUSEMODE_ALWAYS,
		PAUSEMODE_WHENPAUSED
	};

	Entity(void);
	Entity(EntityId id);
	EntityId getId(void) const;
	void updateSprite(void);
	void updateAnimator(const Game *game);
	bool getDamage(const Game *game, int damage);
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
	int health;

	Tick invicibility_end_tick;
	Tick damage_cooldown;

	EntityId target_id;
	std::array<EntityId, ENTITY_MAX_CHILDREN> children;
	EntityId parent_id;
	
	int state;

	uint32_t collision_layer;
	uint32_t collision_mask;
	uint32_t collision_trigger;

	PauseMode pause_mode;

	std::array<Tick, ENTITY_MAX_TIMERS> timers;
	std::array<int, ENTITY_MAX_FLAGS> flags;

	Animator animator;

	private:
		EntityId id;
};

#endif
