#include "engine/Entity.hpp"
#include "engine/Game.hpp"
#include "core/Hitbox.hpp"

#define EPS 0.01f

Entity::Entity(void) {
	id = -1;
	alive = false;
	texture = NULL;

	texture_cell = 0;

	flip_x = false;
	flip_y = false;

	target_id = -1;
	child_id = -1;
	parent_id = -1;

	collision_layer = 0;
	collision_mask = 0;
	collision_trigger = 0;
}

void Entity::render(Game *game) const {
	if(texture == NULL)
		return;

	texture->renderCell(
			game->getContext(),
			(int) (position.x + texture_offset.x),
			(int) (position.y + texture_offset.y),
			texture_cell,
			flip_x,
			flip_y
			);
}

bool Entity::checkCollision(const Vec2& other_pos, const Vec2& other_size) const {
	return Hitbox::checkCollision(
			position,
			size,
			other_pos,
			other_size
			);
}

bool Entity::checkCollision(const Entity& other) const {
	if((collision_mask & other.collision_layer) == 0)
		return false;

	return checkCollision(other.position, other.size);
}

bool Entity::solveCollision(const Entity& other, Axis axis) {
	if((collision_trigger & other.collision_layer) != 0)
		return false;

	if(!checkCollision(other))
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
