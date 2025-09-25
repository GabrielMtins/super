#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "core/Vec2.hpp"
#include "core/Texture.hpp"

class Game;

struct Sprite {
	Sprite(void);
	void setTexture(Texture *texture);
	bool isOnCamera(const Game *game) const;
	void render(Game *game) const;

	Vec2 position;
	Vec2 size;
	Vec2 center;
	float angle;
	Vec2 offset;
	int cell;
	Texture *texture;
	bool hud_element;
	bool ignore_camera;
	bool visible;

	bool flip_x;
	bool flip_y;

	int layer;
};

#endif
