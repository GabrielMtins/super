#ifndef GAME_HPP
#define GAME_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"
#include "engine/EntityList.hpp"
#include "core/ResourceManager.hpp"

#include <array>
#include <unordered_map>

#define MAX_INPUT 64

class Game {
	public:
		enum InputType {
			INPUT_LEFT = 0,
			INPUT_RIGHT,
			INPUT_DOWN,
			INPUT_UP,
			NUM_INPUTS
		};

		Game(void);
		void run(void);
		Context * getContext(void);
		EntityList * getEntityList(void);

		Texture * getTexture(const std::string& name);

		bool getKey(InputType input_type);
		void setKeyInput(InputType input_type, int scancode);

		~Game(void);

	private:
		void loop(void);
		void updateKeyState(void);

		Context context;
		EntityList entity_list;
		ResourceManager resource_manager;

		std::array<bool, MAX_INPUT> pressed;
		std::unordered_map<int, int> input_to_keys;
};

#endif
