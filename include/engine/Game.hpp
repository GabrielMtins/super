#ifndef GAME_HPP
#define GAME_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"
#include "engine/EntityList.hpp"

class Game {
	public:
		Game(void);
		void run(void);
		Context * getContext(void);
		EntityList * getEntityList(void);
		~Game(void);

		Texture texture;

	private:
		void loop(void);

		Context context;
		EntityList entity_list;

};

#endif
