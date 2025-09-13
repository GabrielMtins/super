#include "engine/World.hpp"

#include "core/Console.hpp"
#include "engine/Game.hpp"
#include "engine/EntityList.hpp"

#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

World::World(void) {
	width = 0;
	height = 0;
	texture = NULL;
}

bool World::load(const Context *context, const std::string& filename, Game *game) {
	std::ifstream file;
	json data;

	file.open(filename);

	if(!file.is_open()) {
		console.error("[World] Failed to open world path: " + filename);
		return false;
	}

	console.log("[World] Opened file successfully: " + filename);

	try {
		data = json::parse(file);
	} catch(const json::exception& ex) {
		console.error((std::string) "[World] " + ex.what());
		return false;
	}

	console.log("[World] Parsed file successfully: " + filename);

	try {
		width = data["width"];
		height = data["height"];

		console.log("[World] World has " + std::to_string(width) + "x" + std::to_string(height) + " tiles.");
	} catch(const json::exception& ex) {
		console.error("[World] There's no width or height data in world file.");
		return false;
	}

	try {
		tile_width = data["tilewidth"];
		tile_height = data["tileheight"];
		tile_size = Vec2(tile_width, tile_height);

		screen_tiles_x = context->getInternalWidth() / tile_width + 1;
		screen_tiles_y = context->getInternalHeight() / tile_height + 1;
	} catch(const json::exception& ex) {
		console.error("[World] There's no tile width or tile height data in world file.");
		return false;
	}

	if(width * height > MAX_TILES) {
		console.error("[World] There are more tiles than supported by the engine.");
		return false;
	}

	try {
		json& layers = data["layers"];
		size_t count = 0;

		if(layers.size() > MAX_LAYERS) {
			console.error("[World] There are more layers than supported by the engine.");
			return false;
		}

		for(json& layer : layers) {
			const std::string& type = layer["type"];

			if(type == "tilelayer") {
				std::vector<TileId> vector_tiles = layer["data"];
				std::copy_n(vector_tiles.begin(), vector_tiles.size(), tiles[count++].begin());
			} else if(type == "objectgroup") {
				loadObjects(game, layer["objects"]);
			}
		}
	} catch (const json::exception& ex) {
		console.error((std::string) "[World] " + ex.what());
		return false;
	}

	return true;
}

TileId World::getTile(int i, int j, int layer) const {
	if(outOfBounds(i, j, layer))
		return 0;

	return tiles[layer][i + j * width];
}

void World::setTexture(Texture *texture) {
	this->texture = texture;
}

void World::render(Game *game, int layer) const {
	Context *context;
	int pos_x, pos_y;
	int start_x, start_y;
	TileId current_id;
	const Vec2& camera_position = game->getCameraPosition();

	start_x = floorf(camera_position.x / tile_size.x);
	start_y = floorf(camera_position.y / tile_size.y);

	if(texture == NULL)
		return;

	context = game->getContext();

	for(int j = -1; j <= screen_tiles_y; j++) {
		for(int i = -1; i <= screen_tiles_x; i++) {
			pos_x = (i + start_x) * tile_width - camera_position.x;
			pos_y = (j + start_y) * tile_height - camera_position.y;

			current_id = getTile(i + start_x, j + start_y, layer);

			if(!current_id)
				continue;

			texture->renderCell(
					context,
					pos_x,
					pos_y,
					current_id - 1,
					false,
					false
					);
		}
	}
}

uint32_t World::getCollisionLayer(void) const {
	return collision_layer;
}

void World::setCollisionLayer(uint32_t collision_layer) {
	this->collision_layer = collision_layer;
}

const Vec2& World::getTileSize(void) const {
	return tile_size;
}

void World::setEntityTypeGid(int gid, EntityType entity_type) {
	gid_to_id[gid] = entity_type;
}

void World::setTile(int i, int j, int layer, TileId id) {
	if(outOfBounds(i, j, layer))
		return;

	tiles[layer][i + j * width] = id;
}

bool World::outOfBounds(int i, int j, int layer) const {
	return (i < 0) || (j < 0) || (i >= width) || (j >= height) || (layer < 0) || (layer > MAX_LAYERS); 
}

bool World::loadObjects(Game *game, const json& j) {
	for(const auto& object : j) {
		if(!object.contains("gid"))
			continue;

		const EntityType type = gid_to_id[object.at("gid")];
		Entity *entity = game->getEntityFromId(game->addEntity(type));

		printf("%i\n", type);

		if(object.contains("x")) {
			entity->position.x = object["x"];
		}

		if(object.contains("y")) {
			entity->position.y = object["y"];
		}
	}

	return true;
}
