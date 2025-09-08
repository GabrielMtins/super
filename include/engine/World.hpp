#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <array>

#include "core/Texture.hpp"
#include "core/Vec2.hpp"
#include "core/Context.hpp"

#define MAX_LAYERS 3
#define MAX_TILES (256 * 256)

class Game;

using TileId = uint16_t;

class World {
	public:
		World(void);
		bool load(const Context *context, const std::string& filename);
		TileId getTile(int i, int j, int layer) const;
		void setTexture(Texture *texture);
		void render(Game *game, int layer) const;
		uint32_t getCollisionLayer(void) const;
		void setCollisionLayer(uint32_t collision_layer);
		const Vec2& getTileSize(void) const;

	private:
		void setTile(int i, int j, int layer, TileId value);
		bool outOfBounds(int i, int j, int layer) const;

		int width, height;
		int tile_width, tile_height;
		int screen_tiles_x, screen_tiles_y;

		Vec2 tile_size;
		uint32_t collision_layer;
		Texture *texture;
		std::array<TileId, MAX_TILES> tiles[MAX_LAYERS];
};

#endif
