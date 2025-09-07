#include "core/Hitbox.hpp"

namespace Hitbox {
	bool checkCollision(const Vec2& a_pos, const Vec2& a_size, const Vec2& b_pos, const Vec2& b_size) {
		if(a_pos.x + a_size.x < b_pos.x)
			return false;

		if(a_pos.x > b_pos.x + b_size.x)
			return false;

		if(a_pos.y + a_size.y < b_pos.y)
			return false;

		if(a_pos.y > b_pos.y + b_size.y)
			return false;

		return true;
	}
};

