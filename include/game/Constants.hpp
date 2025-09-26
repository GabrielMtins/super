#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "core/Vec2.hpp"

extern const Vec2 gravity;

namespace InputType {
	enum Input {
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		JUMP,
		FIRE
	};
};

#endif
