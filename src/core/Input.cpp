#include "core/Input.hpp"

#include "core/Context.hpp"

Input::Input(void) {
	for(int i = 0; i < MAX_INPUT_TYPES; i++) {
		pressed[i] = false;
		input_to_key[i] = SDL_SCANCODE_UNKNOWN;
		input_to_button[i] = SDL_CONTROLLER_BUTTON_INVALID;
	}

	for(Tick& i : input_tick_down) {
		i = 9999999;
	}

	for(Tick& i : input_tick_down) {
		i = 9999999;
	}

	mode = MODE_KEYBOARD;

	controller = NULL;
}

void Input::update(Tick tick) {
	switch(mode) {
		case MODE_KEYBOARD:
			updateKeyState(tick);
			break;

		case MODE_GAMEPAD:
			updateButtonState(tick);
			break;
	}
}

void Input::setInputKey(int input, int key) {
	if(!valid(input))
		return;

	input_to_key[input] = key;
}

void Input::setInputButton(int input, int button) {
	if(!valid(input))
		return;

	input_to_button[input] = button;
}

bool Input::getInput(int input) const {
	if(!valid(input))
		return false;

	return pressed[input];
}

bool Input::getInputDown(int input, Tick tick) const {
	if(!valid(input))
		return false;

	return tick == input_tick_down[input];
}

bool Input::getInputUp(int input, Tick tick) const {
	if(!valid(input))
		return false;

	return tick == input_tick_up[input];
}

bool Input::openController(int index) {
	controller = SDL_GameControllerOpen(index);

	return controller != NULL;
}

void Input::closeController(void) {
	SDL_GameControllerClose(controller);
}

void Input::setMode(Mode mode) {
	this->mode = mode;
}

void Input::updateKeyState(Tick tick) {
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_INPUT_TYPES; i++) {
		bool new_state;

		new_state = keys[input_to_key.at(i)];

		if(new_state == true && pressed[i] == false) {
			input_tick_down[i] = tick;
		}
		else if(new_state == false && pressed[i] == true) {
			input_tick_up[i] = tick;
		}

		pressed[i] = new_state;
	}
}

void Input::updateButtonState(Tick tick) {
	if(controller == NULL)
		return;

	for(int i = 0; i < MAX_INPUT_TYPES; i++) {
		bool new_state;

		new_state = SDL_GameControllerGetButton(
				controller,
				(SDL_GameControllerButton) input_to_button[i]
				);

		if(new_state == true && pressed[i] == false) {
			input_tick_down[i] = tick;
		}
		else if(new_state == false && pressed[i] == true) {
			input_tick_up[i] = tick;
		}

		pressed[i] = new_state;
	}
}

bool Input::valid(int input) const {
	return (input >= 0 && input < MAX_INPUT_TYPES);
}
