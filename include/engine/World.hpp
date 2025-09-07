#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <array>

#define MAX_LAYERS 3
#define MAX_TILES (256 * 256)

using TileId = int;

class World {
	public:
		World(void);
		bool load(const std::string& filename);

	private:
		int width, height;
		std::array<TileId, MAX_TILES> tiles[MAX_LAYERS];
};

#endif
