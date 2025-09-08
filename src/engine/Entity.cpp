#include "engine/Entity.hpp"
#include "engine/Game.hpp"
#include "core/Hitbox.hpp"

#define EPS 0.01f

Sprite::Sprite(void) {
	cell = 0;
	texture = NULL;
	flip_x = false;
	flip_y = false;
	hud_element = false;
	layer = 0;
}

void Sprite::setTexture(Texture *texture) {
	this->texture = texture;

	size = Vec2(
			texture->getCellWidth(),
			texture->getCellHeight()
			);
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
			flip_y
			);
}

Entity::Entity(void) {
	id = -1;
	alive = false;

	target_id = -1;
	child_id = -1;
	parent_id = -1;

	collision_layer = 0;
	collision_mask = 0;
	collision_trigger = 0;
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
