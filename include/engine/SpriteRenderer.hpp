#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include "engine/Entity.hpp"
#include "engine/EngineDef.hpp"

class Game;

class SpriteRenderer {
	public:
		SpriteRenderer(void);
		void reset(void);
		void addSpriteToRenderList(const Game *game, const Sprite& sprite);
		void sortSprites(void);
		void renderSprites(Game *game);
		void renderSpritesUntilLayer(Game *game, int layer);

	private:
		static bool spriteComparison(const Sprite& a, const Sprite& b);

		std::array<Sprite, MAX_SPRITES> sprites;
		size_t num_sprites;

		size_t start_sprite_index;
};

#endif
