#ifndef GAME_HPP
#define GAME_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"
#include "engine/EntityList.hpp"
#include "core/ResourceManager.hpp"
#include "core/TextGenerator.hpp"
#include "core/Input.hpp"
#include "engine/World.hpp"
#include "engine/SpriteRenderer.hpp"

#include "engine/EngineDef.hpp"

#include <array>
#include <unordered_map>

class Game {
	public:
		enum MouseButton {
			MOUSEBUTTON_LEFT = 1,
			MOUSEBUTTON_MIDDLE,
			MOUSEBUTTON_RIGHT,
		};

		Game(void);
		void init(const std::string& title, int internal_width, int internal_height, int scale);
		void run(void);
		Context * getContext(void);
		World * getWorld(void);

		void setWorldRenderLayerBg(int layer);
		void setWorldRenderLayerFg(int layer);

		void loadRes(const std::string& filename);
		void loadFontData(const std::string& filename);
		void loadLocale(const std::string& filename, const std::string& locale_name);
		void loadWorld(const std::string& filename);

		Texture * getTexture(const std::string& name);
		Sfx * getSfx(const std::string& name);
		Music * getMusic(const std::string& name);

		Texture * getTextTexture(const std::string& key);

		const Vec2& getCameraPosition(void) const;
		void setCameraPosition(const Vec2& camera_position);

		void pause(void);
		bool isPaused(void) const;

		const Vec2& getScreenDimensions(void) const;
		const Vec2& getMousePosition(void) const;

		bool getInput(int input_type) const;
		bool getInputDown(int input_type) const;
		bool getInputUp(int input_type) const;

		bool getMouseButton(MouseButton mouse_button) const;
		bool getMouseButtonDown(MouseButton mouse_button) const;
		bool getMouseButtonUp(MouseButton mouse_button) const;

		bool openController(int index);
		void closeController(void);
		void setInputMode(Input::Mode mode);
		void setInputKey(int input_type, int scancode);
		void setInputButton(int input_type, int button);

		Tick getCurrentTick(void) const;

		void addHandlerToType(EntityType type, const EntityHandler& handler);
		EntityId addEntity(EntityType type);
		Entity * getEntityFromId(EntityId id);
		void transformEntityToType(EntityId id, EntityType type);

		EntityFoundList findEntity(const Vec2& position, float radius);
		EntityFoundList findEntity(EntityType type);
		EntityFoundList findEntity(EntityType type, const Vec2& position, float radius);
		EntityFoundList findCollision(const Hitbox& hitbox);

		bool checkCollision(const Hitbox& hitbox) const;
		void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b);

		void addSpriteToRenderList(const Sprite& sprite);

		void setFps(uint32_t fps);
		void setMinFps(uint32_t fps);

		uint32_t getCurrentFrame(void) const;

		void quit(void);

	private:
		void loop(void);
		void update(void);
		void render(void);
		void updateKeyState(void);
		void updateMouseState(void);

		Context context;
		EntityList entity_list;
		ResourceManager resource_manager;
		SpriteRenderer sprite_renderer;
		TextGenerator text_generator;
		World world;

		int world_render_layer_bg;
		int world_render_layer_fg;

		Input input;

		Vec2 camera_position;
		Vec2 screen_dimensions;
		Vec2 mouse_position;

		Tick current_tick;
		uint32_t frame_counter;
		float dt;

		bool paused;
		uint32_t old_mouse_state;
		uint32_t mouse_state;

		uint32_t delay_fps;
		float minimum_dt;

		SDL_Color bg;
};

#endif
