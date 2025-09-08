#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include "engine/Entity.hpp"
#define MAX_SPRITES 64

class Game;

class SpriteRenderer {
	public:
		SpriteRenderer(void);
		void reset(void);
		void addSpriteToRenderList(const Game *game, const Sprite& sprite);
		void render(Game *game);

	private:
		static bool spriteComparison(const Sprite& a, const Sprite& b);

		std::array<Sprite, MAX_SPRITES> sprites;
		size_t num_sprites;
};

#endif
