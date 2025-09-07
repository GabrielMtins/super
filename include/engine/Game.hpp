#ifndef GAME_HPP
#define GAME_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"
#include "engine/EntityList.hpp"
#include "core/ResourceManager.hpp"
#include "engine/World.hpp"

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
		void init(const std::string& title, int internal_width, int internal_height);
		void run(void);
		Context * getContext(void);
		EntityList * getEntityList(void);
		World * getWorld(void);

		void loadRes(const std::string& filename);

		Texture * getTexture(const std::string& name);
		Sfx * getSfx(const std::string& name);
		Music * getMusic(const std::string& name);

		const Vec2& getCameraPosition(void) const;
		void setCameraPosition(const Vec2& camera_position);

		const Vec2& getScreenDimensions(void) const;

		bool getKey(InputType input_type) const;
		bool getKeyDown(InputType input_type) const;
		bool getKeyUp(InputType input_type) const;

		void setKeyInput(InputType input_type, int scancode);

		Tick getCurrentTick(void);

		void quit(void);

	private:
		void loop(void);
		void updateKeyState(void);

		Context context;
		EntityList entity_list;
		ResourceManager resource_manager;
		World world;

		Vec2 camera_position;
		Vec2 screen_dimensions;

		std::array<bool, MAX_INPUT> pressed;
		std::array<Tick, MAX_INPUT> input_tick_down;
		std::array<Tick, MAX_INPUT> input_tick_up;

		std::unordered_map<int, int> input_to_keys;
		Tick current_tick;
		float dt;
};

#endif
