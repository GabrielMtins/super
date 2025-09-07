#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "core/Vec2.hpp"

namespace Hitbox {
	bool checkCollision(const Vec2& a_pos, const Vec2& a_size, const Vec2& b_pos, const Vec2& b_size);
};

#endif
