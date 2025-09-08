#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "core/Context.hpp"

class Texture {
	public:
		Texture(void);
		bool load(Context *context, const std::string& filename);
		bool generateText(Context *context, TTF_Font *font, const std::string& text, const SDL_Color& color);
		void setCellSize(int cell_width, int cell_height);
		void renderCell(Context *context, int x, int y, int cell, bool flip_x, bool flip_y);
		void render(Context *context, int x, int y);
		void unload(void);
		int getCellWidth(void) const;
		int getCellHeight(void) const;

	private:
		SDL_Texture *texture;
		int width, height;
		int cell_width, cell_height;
		int num_cells_x;
		std::string path;

		SDL_Rect getCell(int cell);
};

#endif
