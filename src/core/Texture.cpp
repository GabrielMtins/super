#include "core/Texture.hpp"
#include "core/Console.hpp"

#define PI 3.141592f

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

bool Texture::generateText(Context *context, TTF_Font *font, const std::string& text, const SDL_Color& color) {
	SDL_Surface *surface;

	surface = TTF_RenderUTF8_Solid(
			font,
			text.c_str(),
			color
			);

	if(surface == NULL) {
		console.error("[Texture] Failed to generate text: " + text);
		return false;
	}

	texture = SDL_CreateTextureFromSurface(
			context->getRenderer(),
			surface
			);

	path = "GENERATED TEXT - " + text;
	width = surface->w;
	height = surface->h;
	cell_width = surface->w;
	cell_height = surface->h;

	SDL_FreeSurface(surface);

	return true;
}

void Texture::setCellSize(int cell_width, int cell_height) {
	this->cell_width = cell_width;
	this->cell_height = cell_height;

	num_cells_x = width / cell_width;
}

void Texture::renderCell(Context *context, int x, int y, int cell, bool flip_x, bool flip_y) {
	renderCell(
			context,
			x,
			y,
			cell,
			flip_x,
			flip_y,
			cell_width / 2,
			cell_height / 2,
			0.0f
			);
}

void Texture::renderCell(Context *context, int x, int y, int cell, bool flip_x, bool flip_y, int center_x, int center_y, float angle) {
	SDL_Rect src_rect, dst_rect;
	SDL_Point center;
	const SDL_Point *used_center = NULL;
	const SDL_Rect *used_src_rect = NULL;

	dst_rect.x = x;
	dst_rect.y = y;
	dst_rect.w = cell_width;
	dst_rect.h = cell_height;

	if(cell >= 0) {
		src_rect = getCell(cell);
		used_src_rect = &src_rect;
	} else {
		dst_rect.w = width;
		dst_rect.h = height;
	}

	center.x = center_x;
	center.y = center_y;

	if(center_x >= 0 && center_y >= 0) {
		used_center = &center;
	}

	SDL_RenderCopyEx(
			context->getRenderer(),
			texture,
			used_src_rect,
			&dst_rect,
			angle * 180.0f / PI,
			used_center,
			(SDL_RendererFlip) ((flip_x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flip_y ?	SDL_FLIP_VERTICAL : SDL_FLIP_NONE))
			);
}

void Texture::render(Context *context, int x, int y) {
	renderCell(
			context,
			x,
			y,
			-1,
			false,
			false
			);

}

void Texture::unload(void) {
	if(texture != NULL) {
		console.log("[Texture] Unloading texture: " + path);
		SDL_DestroyTexture(texture);
	}
}

int Texture::getCellWidth(void) const {
	return cell_width;
}

int Texture::getCellHeight(void) const {
	return cell_height;
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
