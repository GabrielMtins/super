#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "core/Context.hpp"

class Texture {
	public:
		Texture(void);
		bool load(Context *context, const std::string& filename);
		void setCellSize(int cell_width, int cell_height);
		void renderCell(Context *context, int x, int y, int cell, bool flip_x, bool flip_y);
		void unload(void);

	private:
		SDL_Texture *texture;
		int width, height;
		int cell_width, cell_height;
		int num_cells_x;

		SDL_Rect getCell(int cell);
};

#endif
