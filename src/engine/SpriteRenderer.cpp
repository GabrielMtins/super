#include "engine/SpriteRenderer.hpp"
#include "engine/Game.hpp"

#include <algorithm>

SpriteRenderer::SpriteRenderer(void) {
	num_sprites = 0;
}

void SpriteRenderer::reset(void) {
	num_sprites = 0;
}

void SpriteRenderer::addSpriteToRenderList(const Game *game, const Sprite& sprite) {
	if(sprite.isOnCamera(game) && num_sprites < MAX_SPRITES) {
		sprites[num_sprites++] = sprite;
	}
}

void SpriteRenderer::render(Game *game) {
	std::sort(
			sprites.begin(),
			sprites.begin() + num_sprites,
			spriteComparison
			);

	for(size_t i = 0; i < num_sprites; i++) {
		sprites[i].render(game);
	}

	reset();
}

bool SpriteRenderer::spriteComparison(const Sprite& a, const Sprite& b) {
	if(a.layer < b.layer)
		return true;
	else if(a.layer > b.layer)
		return false;

	return a.position.y + a.size.y < b.position.y + b.size.y;
}
