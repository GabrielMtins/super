#ifndef GAME_HPP
#define GAME_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"
#include "engine/EntityList.hpp"
#include "core/ResourceManager.hpp"

class Game {
	public:
		Game(void);
		void run(void);
		Context * getContext(void);
		EntityList * getEntityList(void);

		Texture * getTexture(const std::string& name);

		~Game(void);

	private:
		void loop(void);

		Context context;
		EntityList entity_list;
		ResourceManager resource_manager;

};

#endif
