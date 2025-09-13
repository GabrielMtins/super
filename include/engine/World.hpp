#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <array>
#include <unordered_map>

#include "core/Texture.hpp"
#include "core/Vec2.hpp"
#include "core/Context.hpp"

#include "engine/EngineDef.hpp"

#include "nlohmann/json_fwd.hpp"

class Game;
class EntityList;

using TileId = uint16_t;

class World {
	public:
		World(void);
		bool load(const Context *context, const std::string& filename, Game *game);
		TileId getTile(int i, int j, int layer) const;
		void setTexture(Texture *texture);
		void render(Game *game, int layer) const;
		uint32_t getCollisionLayer(void) const;
		void setCollisionLayer(uint32_t collision_layer);
		const Vec2& getTileSize(void) const;
		void setEntityTypeGid(int gid, EntityType entity_type);

	private:
		void setTile(int i, int j, int layer, TileId value);
		bool outOfBounds(int i, int j, int layer) const;
		bool loadObjects(Game *game, const nlohmann::json& j);

		int width, height;
		int tile_width, tile_height;
		int screen_tiles_x, screen_tiles_y;

		Vec2 tile_size;
		uint32_t collision_layer;
		Texture *texture;
		std::array<TileId, MAX_TILES> tiles[MAX_LAYERS];
		std::unordered_map<int, EntityType> gid_to_id;
};

#endif
