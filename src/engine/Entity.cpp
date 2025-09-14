#include "engine/Entity.hpp"
#include "engine/Game.hpp"
#include "core/Hitbox.hpp"
#include "engine/World.hpp"

#define EPS 0.50f
#define MAX_DELTA 999.0f

Entity::Entity(void) {
	id = -1;
	alive = false;

	target_id = -1;
	parent_id = -1;

	for(EntityId& i : children)
		i = -1;

	collision_layer = 0;
	collision_mask = 0;
	collision_trigger = 0;
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

bool Entity::checkCollision(const Vec2& other_pos, const Vec2& other_size) const {
	return Hitbox::checkCollision(
			position,
			size,
			other_pos,
			other_size
			);
}

void Entity::updateSprite(void) {
	sprite.position = position - sprite.offset;
	sprite.cell = animator.getCurrentCell();
}

void Entity::updateAnimator(const Game *game) {
	animator.update(game);
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

bool Entity::checkCollision(const Entity& other) const {
	bool found_collision_mask = (collision_mask & other.collision_layer);
	bool found_collision_trigger = (collision_trigger & other.collision_layer);

	if(!(found_collision_trigger || found_collision_mask))
		return false;

	return checkCollision(other.position, other.size);
}

bool Entity::solveCollision(const Entity& other, Axis axis) {
	if((collision_trigger & other.collision_layer) != 0)
		return false;

	if(axis == AXIS_X) {
		if(velocity.x > 0.0f) {
			position.x = other.position.x - size.x - EPS;
		}
		else {
			position.x = other.position.x + other.size.x + EPS;
		}

		velocity.x = 0.0f;
	}
	else {
		if(velocity.y > 0.0f) {
			position.y = other.position.y - size.y - EPS;
		}
		else {
			position.y = other.position.y + other.size.y + EPS;
		}

		velocity.y = 0.0f;
	}

	return true;
}

bool Entity::checkCollision(const World* world) const {
	int min_x, max_x, min_y, max_y;
	Vec2 tile_pos;
	bool found_collision_trigger, found_collision_mask;

	const Vec2& tile_size = world->getTileSize();

	found_collision_mask = collision_mask & world->getCollisionLayer();
	found_collision_trigger = collision_trigger & world->getCollisionLayer();

	if(!(found_collision_trigger || found_collision_mask)) {
		return false;
	}

	min_x = floorf(position.x / tile_size.x);
	min_y = floorf(position.y / tile_size.y);
	max_x = ceilf((position.x + size.x) / tile_size.x);
	max_y = ceilf((position.y + size.y) / tile_size.y);

	for(int j = min_y; j < max_y; j++) {
		for(int i = min_x; i < max_x; i++) {
			if(!world->getTile(i, j, WORLD_LAYER_FG)) {
				continue;
			}

			tile_pos = Vec2(i, j) * tile_size;

			if(checkCollision(tile_pos, tile_size)){
				return true;
			}
		}
	}

	return false;
}

bool Entity::solveCollision(const World* world, Axis axis) {
	int min_x, max_x, min_y, max_y;
	const Vec2& tile_size = world->getTileSize();

	if(collision_trigger & world->getCollisionLayer())
		return false;

	min_x = ceilf(position.x / tile_size.x);
	min_y = ceilf(position.y / tile_size.y);
	max_x = floorf((position.x + size.x) / tile_size.x);
	max_y = floorf((position.y + size.y) / tile_size.y);

	if(axis == AXIS_X) {
		if(velocity.x > 0.0f) {
			position.x = (max_x) * tile_size.x - size.x - EPS;  
		}
		else {
			position.x = min_x * tile_size.x + EPS;
		}

		velocity.x = 0.0f;
	}
	else {
		if(velocity.y > 0.0f) {
			position.y = (max_y) * tile_size.y - size.y - EPS;  
		}
		else {
			position.y = min_y * tile_size.y + EPS;
		}

		velocity.y = 0.0f;
	}

	return true;
}

bool Entity::solveCollision(const Entity& other) {
	Vec2 dir(MAX_DELTA, MAX_DELTA);
	float left, right, top, bottom;
	float other_left, other_right, other_top, other_bottom;

	if(collision_trigger & other.collision_layer)
		return false;

	left = position.x;
	right = position.x + size.x;
	bottom = position.y;
	top = position.y + size.y;

	other_left = other.position.x;
	other_right = other.position.x + other.size.x;
	other_bottom = other.position.y;
	other_top = other.position.y + other.size.y;

	if(left < other_right && right > other_right) {
		dir.x = other_right - left;
	} else if(right > other_left && left < other_left) {
		dir.x = other_left - right;
	}

	if(bottom < other_top && top > other_top) {
		dir.y = other_top - bottom;
	} else if(top > other_bottom && bottom < other_bottom) {
		dir.y = other_bottom - top;
	}
	
	dir *= 1.10f;

	if(fabsf(dir.x) < fabsf(dir.y) && fabsf(dir.x) < MAX_DELTA) {
		position.x += dir.x;
	} else if(fabsf(dir.y) < MAX_DELTA){
		position.y += dir.y;
	}

	return true;
}
