#include "engine/Entity.hpp"
#include "engine/Game.hpp"
#include "core/Hitbox.hpp"
#include "engine/World.hpp"

#define EPS 0.50f
#define MAX_DELTA 999.0f

Entity::Entity(void) {
	id = -1;
	alive = false;

	target_id = 0;
	parent_id = 0;

	for(EntityId& i : children)
		i = 0;

	for(Tick& i : timers)
		i = 0;

	for(int& i : counters)
		i = 0;

	pause_mode = PAUSEMODE_PAUSABLE;

	damage_cooldown = 0;
	invicibility_end_tick = 0;
	health = 100;
}

Entity::Entity(EntityId id) : Entity() {
	this->id = id;
}

EntityId Entity::getId(void) const {
	return id;
}

void Entity::updateSprite(void) {
	sprite.position = hitbox.position - sprite.offset;
	sprite.cell = animator.getCurrentCell();
}

void Entity::updateAnimator(const Game *game) {
	animator.update(game);
}

void Entity::updateCenter(void) {
	center = hitbox.position + hitbox.size * 0.5f;
}

bool Entity::getDamage(const Game *game, int damage) {
	bool success;
	const Tick current_tick = game->getCurrentTick();

	success = false;

	if(current_tick > invicibility_end_tick) {
		health -= damage;

		invicibility_end_tick = current_tick + damage_cooldown;
		success = true;
	}

	if(health <= 0) {
		alive = false;
	}

	return success;
}
