#ifndef GAME_HPP
#define GAME_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"
#include "engine/EntityList.hpp"
#include "core/ResourceManager.hpp"
#include "core/TextGenerator.hpp"
#include "engine/World.hpp"
#include "engine/SpriteRenderer.hpp"

#include "engine/EngineDef.hpp"

#include <array>
#include <unordered_map>

class Game {
	public:
		enum InputType {
			INPUT_LEFT = 0,
			INPUT_RIGHT,
			INPUT_DOWN,
			INPUT_UP,
			INPUT_JUMP,
			INPUT_FIRE,
			NUM_INPUTS
		};

		enum MouseButton {
			MOUSEBUTTON_LEFT = 1,
			MOUSEBUTTON_MIDDLE,
			MOUSEBUTTON_RIGHT,
		};

		Game(void);
		void init(const std::string& title, int internal_width, int internal_height);
		void run(void);
		Context * getContext(void);
		World * getWorld(void);

		void loadRes(const std::string& filename);
		void loadFont(const std::string& filename, int size);
		void loadLocale(const std::string& filename, const std::string& locale_name);
		void loadWorld(const std::string& filename);

		Texture * getTexture(const std::string& name);
		Sfx * getSfx(const std::string& name);
		Music * getMusic(const std::string& name);

		const Vec2& getCameraPosition(void) const;
		void setCameraPosition(const Vec2& camera_position);

		void pause(void);
		bool isPaused(void) const;

		const Vec2& getScreenDimensions(void) const;
		const Vec2& getMousePosition(void) const;

		bool getKey(InputType input_type) const;
		bool getKeyDown(InputType input_type) const;
		bool getKeyUp(InputType input_type) const;

		bool getMouseButton(MouseButton mouse_button) const;
		bool getMouseButtonDown(MouseButton mouse_button) const;
		bool getMouseButtonUp(MouseButton mouse_button) const;

		void setKeyInput(InputType input_type, int scancode);

		Tick getCurrentTick(void) const;

		void addHandlerToType(EntityType type, const EntityHandler& handler);
		EntityId addEntity(EntityType type);
		Entity * getEntityFromId(EntityId id);

		EntityFoundList findEntity(const Vec2& position, float radius);
		EntityFoundList findEntity(EntityType type);
		EntityFoundList findEntity(EntityType type, const Vec2& position, float radius);

		bool checkCollision(const Hitbox& hitbox) const;
		void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b);

		void quit(void);

	private:
		void loop(void);
		void updateKeyState(void);
		void updateMouseState(void);

		Context context;
		EntityList entity_list;
		ResourceManager resource_manager;
		SpriteRenderer sprite_renderer;
		TextGenerator text_generator;
		World world;

		Vec2 camera_position;
		Vec2 screen_dimensions;
		Vec2 mouse_position;

		std::array<bool, MAX_INPUT> pressed;
		std::array<Tick, MAX_INPUT> input_tick_down;
		std::array<Tick, MAX_INPUT> input_tick_up;

		std::unordered_map<int, int> input_to_keys;
		Tick current_tick;
		float dt;

		bool paused;
		uint32_t old_mouse_state;
		uint32_t mouse_state;

		SDL_Color bg;
};

#endif
