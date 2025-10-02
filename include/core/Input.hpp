#ifndef INPUT_HPP
#define INPUT_HPP

#include <array>
#include <unordered_map>
#include <bitset>

#include "core/CoreDef.hpp"

#include <SDL2/SDL.h>

class Input {
	public:
		enum Mode {
			MODE_KEYBOARD,
			MODE_GAMEPAD
		};

		Input(void);
		
		void update(Tick tick);
		void setInputKey(int input, int key);
		void setInputButton(int input, int button);
		bool getInput(int key) const;
		bool getInputDown(int key, Tick tick) const;
		bool getInputUp(int key, Tick tick) const;

		bool openController(int index);
		void closeController(void);

		void setMode(Mode mode);
	
	private:
		void updateKeyState(Tick tick);
		void updateButtonState(Tick tick);
		bool valid(int input) const;

		std::array<int, MAX_INPUT_TYPES> input_to_key;
		std::array<int, MAX_INPUT_TYPES> input_to_button;

		std::bitset<MAX_INPUT_TYPES> pressed;

		std::array<Tick, MAX_INPUT_TYPES> input_tick_down;
		std::array<Tick, MAX_INPUT_TYPES> input_tick_up;

		SDL_GameController *controller;
		Mode mode;
};

#endif
