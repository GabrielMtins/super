#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "core/Vec2.hpp"

#include <cstdint>

struct Hitbox {
	public:
		enum CollisionType {
			COLLISION_SOLID,
			COLLISION_ONEWAY_UP,
		};
	
		Hitbox(void);
	
		bool checkCollision(const Hitbox& other) const;
		bool checkCollision(const Hitbox& other, const Vec2& velocity) const;

		void solveCollision(const Hitbox& other, Vec2& velocity);
	
		CollisionType type;
	
		Vec2 position;
		Vec2 size;
	
		uint32_t layer;
		uint32_t mask;

	private:
		bool checkCollisionSolid(const Hitbox& other) const;
		bool checkCollisionOneWayUp(const Hitbox& other, const Vec2& velocity) const;
		
		void solveCollisionSolid(const Hitbox& other, Vec2& velocity);
		void solveCollisionOneWayUp(const Hitbox& other, Vec2& velocity);
};

#endif
