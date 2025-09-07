#include "core/Texture.hpp"
#include "core/Console.hpp"

Texture::Texture(void) {
	texture = NULL;
	path = "";
}

bool Texture::load(Context *context, const std::string& filename) {
	texture = IMG_LoadTexture(
			context->getRenderer(),
			filename.c_str()
			);

	path = filename;

	if(texture == NULL) {
		console.error("[Texture] Failed to load texture: " + filename);
		return false;
	}

	SDL_QueryTexture(
			texture,
			NULL,
			NULL,
			&width,
			&height
			);

	console.log("[Texture] Loaded texture: " + filename);
	return true;
}

void Texture::setCellSize(int cell_width, int cell_height) {
	this->cell_width = cell_width;
	this->cell_height = cell_height;

	num_cells_x = width / cell_width;
}

void Texture::renderCell(Context *context, int x, int y, int cell, bool flip_x, bool flip_y) {
	SDL_Rect src_rect, dst_rect;
	const SDL_Rect *used_src_rect = NULL;

	if(cell >= 0) {
		src_rect = getCell(cell);
		used_src_rect = &src_rect;
	}

	dst_rect.x = x;
	dst_rect.y = y;
	dst_rect.w = cell_width;
	dst_rect.h = cell_height;

	SDL_RenderCopyEx(
			context->getRenderer(),
			texture,
			used_src_rect,
			&dst_rect,
			0,
			NULL,
			(SDL_RendererFlip) ((flip_x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flip_y ?	SDL_FLIP_VERTICAL : SDL_FLIP_NONE))
			);
}

void Texture::unload(void) {
	if(texture != NULL) {
		console.log("[Texture] Unloading texture: " + path);
		SDL_DestroyTexture(texture);
	}
}

SDL_Rect Texture::getCell(int cell) {
	int cell_x, cell_y;
	SDL_Rect rect = {0, 0, 0, 0};

	cell_x = cell % num_cells_x;
	cell_y = cell / num_cells_x;

	rect.x = cell_x * cell_width;
	rect.y = cell_y * cell_height;
	rect.w = cell_width;
	rect.h = cell_height;

	return rect;
}
