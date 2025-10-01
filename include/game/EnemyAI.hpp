#ifndef ENEMYAI_HPP
#define ENEMYAI_HPP

#include "game/CustomEntities.hpp"

namespace EnemyAI {
	bool aboutToCollide(const Game *game, const Entity *entity);
	bool willFall(const Game *game, const Entity *entity);

	bool canWalk(const Game *game, const Entity *entity);

	bool checkCollisionThisFrame(const Game *game);

};

#endif
