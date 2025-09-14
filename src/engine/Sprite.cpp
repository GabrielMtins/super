#include "engine/Sprite.hpp"
#include "engine/Game.hpp"
#include "core/Hitbox.hpp"

Sprite::Sprite(void) {
	cell = 0;
	texture = NULL;
	flip_x = false;
	flip_y = false;
	hud_element = false;
	ignore_camera = false;
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

	if(!ignore_camera) {
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
	
	if(!ignore_camera) {
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

