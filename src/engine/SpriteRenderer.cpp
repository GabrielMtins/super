#include "engine/SpriteRenderer.hpp"
#include "engine/Game.hpp"

#include <algorithm>

SpriteRenderer::SpriteRenderer(void) {
	reset();
}

void SpriteRenderer::reset(void) {
	num_sprites = 0;
	num_hud = 0;
	start_sprite_index = 0;
}

void SpriteRenderer::addSpriteToRenderList(const Game *game, const Sprite& sprite) {
	if(!sprite.isOnCamera(game))
		return;

	if(!sprite.hud_element && num_sprites < MAX_SPRITES) {
		sprites[num_sprites++] = sprite;
	} else if(sprite.hud_element && num_hud < MAX_SPRITES){
		hud[num_hud++] = sprite;
	}
}

void SpriteRenderer::renderSprites(Game *game) {
	std::sort(
			sprites.begin(),
			sprites.begin() + num_sprites,
			spriteComparison
			);

	for(size_t i = 0; i < num_sprites; i++) {
		sprites[i].render(game);
	}
}

void SpriteRenderer::renderSpritesUntilLayer(Game *game, int layer) {
	while(sprites[start_sprite_index].layer < layer) {
		sprites[start_sprite_index].render(game);
		start_sprite_index++;
	}
}

void SpriteRenderer::renderHud(Game *game) {
	std::sort(
			hud.begin(),
			hud.begin() + num_hud,
			spriteComparison
			);

	for(size_t i = 0; i < num_hud; i++) {
		hud[i].render(game);
	}
}

bool SpriteRenderer::spriteComparison(const Sprite& a, const Sprite& b) {
	if(a.layer < b.layer)
		return true;
	else if(a.layer > b.layer)
		return false;

	return a.position.y + a.size.y < b.position.y + b.size.y;
}
