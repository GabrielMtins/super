#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Hitbox.hpp"
#include "core/Vec2.hpp"
#include "core/Texture.hpp"

#include "engine/EngineDef.hpp"
#include "engine/Sprite.hpp"
#include "engine/Animator.hpp"

#include <array>

class Game;
class World;

struct Entity {
	public:
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
	
		Vec2 position;
		Hitbox hitbox;
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
	
		PauseMode pause_mode;
	
		std::array<Tick, ENTITY_MAX_TIMERS> timers;
		std::array<int, ENTITY_MAX_FLAGS> flags;
	
		Animator animator;

	private:
		EntityId id;
};

#endif
