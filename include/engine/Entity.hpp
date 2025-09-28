#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Hitbox.hpp"
#include "core/Vec2.hpp"
#include "core/Texture.hpp"

#include "engine/EngineDef.hpp"
#include "engine/Sprite.hpp"
#include "engine/Animator.hpp"

#include <array>
#include <bitset>
#include <string>

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
		void updateSprite(const Game *game);
		void updateAnimator(const Game *game);
		void updateCenter(void);

		bool getDamage(const Game *game, int damage);

		std::string name;
	
		Hitbox hitbox;
		Vec2 center;
		Vec2 velocity;
		Vec2 contact_velocity;
		Vec2 direction;
	
		Sprite sprite;
	
		EntityType type;

		/**
		 * Essa variável é definida como o tipo anterior de uma entidade,
		 * quando este tipo é mudado pela função transformEntityToType().
		 */
		EntityType old_type;
	
		bool alive;
		int health;
	
		Tick invicibility_end_tick;
		Tick damage_cooldown;
	
		EntityId target_id;
		std::array<EntityId, ENTITY_MAX_CHILDREN> children;
		EntityId parent_id;
		
		int state;
	
		PauseMode pause_mode;
		bool blink_when_damaged;
	
		std::array<Tick, ENTITY_MAX_TIMERS> timers;
		std::array<int, ENTITY_MAX_COUNTERS> counters;
		std::bitset<ENTITY_MAX_FLAGS> flags;
	
		Animator animator;

	private:
		EntityId id;

		friend class EntityList;
};

#endif
