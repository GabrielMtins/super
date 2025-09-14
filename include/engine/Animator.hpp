#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "core/Context.hpp"

class Game;

class Animator {
	public:
		Animator(void);
		void setAnimation(const int *frames, int delay);
		void update(const Game *game);
		int getCurrentCell(void) const;

	private:
		const int *frames;
		int current_frame;
		int delay;

		Tick next_tick;
};

#endif
