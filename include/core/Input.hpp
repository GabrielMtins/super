#ifndef INPUT_HPP
#define INPUT_HPP

#include <array>
#include <unordered_map>
#include <bitset>

#include "core/CoreDef.hpp"

class Input {
	public:
		Input(void);
		
		void update(Tick tick);
		void setInputKey(int input, int key);
		bool getInput(int key) const;
		bool getInputDown(int key, Tick tick) const;
		bool getInputUp(int key, Tick tick) const;
	
	private:
		void updateKeyState(Tick tick);
		bool valid(int input) const;

		std::array<int, MAX_INPUT_TYPES> input_to_key;
		std::bitset<MAX_INPUT_TYPES> pressed;

		std::array<Tick, MAX_INPUT_TYPES> input_tick_down;
		std::array<Tick, MAX_INPUT_TYPES> input_tick_up;
};

#endif
