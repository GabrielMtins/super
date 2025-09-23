#include "core/Hitbox.hpp"

#define ONEWAY_EPS 2.0f
#define MAX_DELTA 999.0f

Hitbox::Hitbox(void) {
	type = COLLISION_SOLID;
	layer = 0;
	mask = 0;
}

bool Hitbox::checkCollision(const Hitbox& other) const {
	return checkCollision(other, Vec2::zero);
}

bool Hitbox::checkCollision(const Hitbox& other, const Vec2& velocity) const {
	if((mask & other.layer) == 0)
		return false;

	if(type != COLLISION_SOLID)
		return false;

	switch(other.type) {
		case COLLISION_SOLID:
			return checkCollisionSolid(other);
			break;

		case COLLISION_ONEWAY_UP:
			return checkCollisionOneWayUp(other, velocity);
			break;
	}

	return false;
}

void Hitbox::solveCollision(const Hitbox& other) {
	switch(other.type) {
		case COLLISION_SOLID:
			solveCollisionSolid(other);
			break;

		case COLLISION_ONEWAY_UP:
			solveCollisionOneWayUp(other);
			break;
	}
}

bool Hitbox::checkCollisionSolid(const Hitbox& other) const {
	if(position.x + size.x < other.position.x)
		return false;

	if(position.y + size.y < other.position.y)
		return false;

	if(position.x > other.position.x + other.size.x)
		return false;

	if(position.y > other.position.y + other.size.y)
		return false;

	return true;
}

bool Hitbox::checkCollisionOneWayUp(const Hitbox& other, const Vec2& velocity) const {
	if(position.x + size.x < other.position.x)
		return false;

	if(position.x > other.position.x + other.size.x)
		return false;

	if(position.y + size.y < other.position.y)
		return false;

	if(position.y + size.y < other.position.y + ONEWAY_EPS && velocity.y > 0.0f)
		return true;

	return false;
}

void Hitbox::solveCollisionSolid(const Hitbox& other) {
	Vec2 dir(MAX_DELTA, MAX_DELTA);
	float left, right, top, bottom;
	float other_left, other_right, other_top, other_bottom;

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
}

void Hitbox::solveCollisionOneWayUp(const Hitbox& other) {
	position.y = other.position.y - size.y;
}
