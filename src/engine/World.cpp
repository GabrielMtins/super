#include "engine/World.hpp"

#include "core/Console.hpp"
#include "engine/Game.hpp"

#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

World::World(void) {
	width = 0;
	height = 0;
	texture = NULL;
}

bool World::load(const std::string& filename) {
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
			std::vector<TileId> vector_tiles = layer["data"];
			std::copy_n(vector_tiles.begin(), vector_tiles.size(), tiles[count++].begin());
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

void World::render(Game *game, int layer) {
	Context *context;
	int pos_x, pos_y;
	TileId current_id;

	if(texture == NULL)
		return;

	context = game->getContext();

	for(int j = 0; j < 15; j++) {
		for(int i = 0; i < 40; i++) {
			pos_x = i * tile_width;
			pos_y = j * tile_height;

			current_id = getTile(i, j, layer) - 1;

			if(current_id == -1)
				continue;

			texture->renderCell(
					context,
					pos_x,
					pos_y,
					current_id,
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

void World::setTile(int i, int j, int layer, TileId id) {
	if(outOfBounds(i, j, layer))
		return;

	tiles[layer][i + j * width] = id;
}

bool World::outOfBounds(int i, int j, int layer) const {
	return (i < 0) || (j < 0) || (i >= width) || (j >= height) || (layer < 0) || (layer > MAX_LAYERS); 
}
