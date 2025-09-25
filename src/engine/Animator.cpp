#include "engine/Animator.hpp"
#include "engine/Game.hpp"

Animator::Animator(void) {
	frames = NULL;
	delay = 0;
	current_frame = -1;

	next_tick = 0;
}

void Animator::setAnimation(const int *frames, int delay) {
	this->delay = delay;

	if(this->frames == frames){
		return;
	}

	current_frame = -1;
	this->frames = frames;
	this->next_tick = 0;
}

void Animator::update(const Game *game) {
	const Tick current_tick = game->getCurrentTick();

	if(current_tick < next_tick)
		return;

	next_tick = current_tick + delay;

	if(frames == NULL)
		return;

	if(frames[++current_frame] == -1)
		current_frame = 0;
}

int Animator::getCurrentCell(void) const {
	if(frames == NULL)
		return 0;

	if(current_frame < 0)
		return 0;

	return frames[current_frame];
}

bool Animator::active(void) const {
	return frames != NULL;
}
