#include "engine/Entity.hpp"
#include "engine/Game.hpp"
#include "core/Hitbox.hpp"

#define EPS 0.50f

Sprite::Sprite(void) {
	cell = 0;
	texture = NULL;
	flip_x = false;
	flip_y = false;
	hud_element = false;
	layer = 0;
	angle = 0.0f;
}

void Sprite::setTexture(Texture *texture) {
	this->texture = texture;

	size = Vec2(
			texture->getCellWidth(),
			texture->getCellHeight()
			);

	center = size / 2;
}

bool Sprite::isOnCamera(const Game *game) const {
	Vec2 render_position;

	if(texture == NULL)
		return false;

	render_position = position + offset;

	if(!hud_element) {
		render_position -= game->getCameraPosition();
	}

	return Hitbox::checkCollision(
			render_position,
			size,
			Vec2::zero,
			game->getScreenDimensions()
			);
}

void Sprite::render(Game *game) const {
	Vec2 render_position;

	if(texture == NULL)
		return;

	render_position = position;
	
	if(!hud_element) {
		render_position -= game->getCameraPosition();
	}

	texture->renderCell(
			game->getContext(),
			(int) roundf(render_position.x),
			(int) roundf(render_position.y),
			cell,
			flip_x,
			flip_y,
			center.x,
			center.y,
			angle
			);
}

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
			if(!world->getTile(i, j, 0)) {
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
	Vec2 dir(999.0f, 999.0f);
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

	if(fabsf(dir.x) < fabsf(dir.y)) {
		position.x += dir.x;
		//velocity.x = 0.0f;
	} else {
		position.y += dir.y;
		//velocity.y = 0.0f;
	}

	return true;
}
