#include "engine/Sprite.hpp"
#include "engine/Game.hpp"
#include "core/Hitbox.hpp"

Sprite::Sprite(void) {
	cell = 0;
	texture = NULL;
	flip_x = false;
	flip_y = false;
	ignore_camera = false;
	layer = 0;
	angle = 0.0f;
	visible = true;
}

void Sprite::setTexture(Texture *texture) {
	if(texture == NULL) {
		this->texture = NULL;
		return;
	}

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

	if(!visible)
		return false;

	render_position = position + offset;

	if(!ignore_camera) {
		render_position -= game->getCameraPosition();
	}

	if(render_position.x + size.x < 0.0f)
		return false;

	if(render_position.x >= game->getScreenDimensions().x)
		return false;

	if(render_position.y + size.y < 0.0f)
		return false;

	if(render_position.y >= game->getScreenDimensions().y)
		return false;

	return true;
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
			(render_position.x),
			(render_position.y),
			cell,
			flip_x,
			flip_y,
			center.x,
			center.y,
			angle
			);
}

