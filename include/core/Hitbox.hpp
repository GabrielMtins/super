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
	
		/**
		 * Retorna true caso haja colisão com a outra hitbox.
		 */
		bool checkCollision(const Hitbox& other) const;

		/**
		 * Retorna true caso haja colisão com a outra hitbox.
		 * O parâmetro de velocidade é utilizado para calcular
		 * colisão com plataformas semisólidas.
		 */
		bool checkCollision(const Hitbox& other, const Vec2& velocity) const;

		/**
		 * Resolve a colisão entre duas hitboxes.
		 * A velocidade é passada como referência, pois ela será modificada
		 * dependendo da superfície da colisão.
		 */
		void solveCollision(const Hitbox& other, Vec2& velocity);
	
		/**
		 * O tipo da hitbox, seguindo a enumeração CollisionType.
		 */
		CollisionType type;
	
		Vec2 position;
		Vec2 size;
	
		/**
		 * Variável que é uma máscara indicando quais camadas a hitbox
		 * está.
		 */
		uint32_t layer;
		/**
		 * Variável que é uma máscara indicando com quais camadas a hitbox
		 * colide.
		 */
		uint32_t mask;

	private:
		bool checkCollisionSolid(const Hitbox& other) const;
		bool checkCollisionOneWayUp(const Hitbox& other, const Vec2& velocity) const;
		
		void solveCollisionSolid(const Hitbox& other, Vec2& velocity);
		void solveCollisionOneWayUp(const Hitbox& other, Vec2& velocity);
};

#endif
