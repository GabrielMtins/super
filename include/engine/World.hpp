#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <array>
#include <unordered_map>

#include "engine/Axis.hpp"
#include "core/Hitbox.hpp"

#include "core/Texture.hpp"
#include "core/Vec2.hpp"
#include "core/Context.hpp"

#include "engine/EngineDef.hpp"

#include "nlohmann/json_fwd.hpp"

class Game;
class EntityList;

using TileId = uint16_t;
using LoadObjectCallback = bool (*)(Game *, const nlohmann::json&);

class World {
	public:
		World(void);
		void setLoadObjectCallback(LoadObjectCallback callback);
		bool load(const Context *context, const std::string& filename, Game *game);
		TileId getTile(int i, int j, int layer) const;
		void setTexture(Texture *texture);
		void render(Game *game, int layer) const;
		uint32_t getCollisionLayer(void) const;
		void setCollisionLayer(uint32_t collision_layer);
		const Vec2& getTileSize(void) const;

		bool checkCollision(const Hitbox& hitbox) const;
		void solveCollision(Hitbox& hitbox, const Vec2& velocity, Axis::Type axis) const;

	private:
		void setTile(int i, int j, int layer, TileId value);
		bool outOfBounds(int i, int j, int layer) const;

		LoadObjectCallback load_object_callback;

		int width, height;
		int tile_width, tile_height;
		int screen_tiles_x, screen_tiles_y;

		Vec2 tile_size;
		uint32_t collision_layer;
		Texture *texture;
		std::array<TileId, MAX_TILES> tiles[MAX_LAYERS];
};

#endif
