#include "engine/World.hpp"

#include "core/Console.hpp"
#include "engine/Game.hpp"
#include "engine/EntityList.hpp"

#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

#define EPS 0.01f

using json = nlohmann::json;

World::World(void) {
	reset();
	texture = NULL;
	load_object_callback = NULL;
}

void World::reset(void) {
	width = 0;
	height = 0;
}

void World::setLoadObjectCallback(LoadObjectCallback callback) {
	load_object_callback = callback;
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
				if(load_object_callback != NULL) {
					const json& objects = layer["objects"];

					for(const json& obj : objects)
						load_object_callback(game, obj);
				}
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
	float pos_x, pos_y;
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
			pos_x = ((float) (i + start_x) * tile_width - camera_position.x);
			pos_y = ((float) (j + start_y) * tile_height - camera_position.y);

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

bool World::checkCollision(const Hitbox& hitbox) const {
	int min_x, max_x, min_y, max_y;
	Hitbox tile_hitbox;
	tile_hitbox.size = tile_size;
	tile_hitbox.layer = collision_layer;

	if(!(hitbox.mask & collision_layer)) {
		return false;
	}

	min_x = floorf(hitbox.position.x / tile_size.x);
	min_y = floorf(hitbox.position.y / tile_size.y);
	max_x = ceilf((hitbox.position.x + hitbox.size.x) / tile_size.x);
	max_y = ceilf((hitbox.position.y + hitbox.size.y) / tile_size.y);

	for(int j = min_y; j < max_y; j++) {
		for(int i = min_x; i < max_x; i++) {
			if(!getTile(i, j, WORLD_LAYER_FG)) {
				continue;
			}

			tile_hitbox.position = Vec2(i, j) * tile_size;

			if(hitbox.checkCollision(tile_hitbox)){
				return true;
			}
		}
	}

	return false;
}

void World::solveCollision(Hitbox& hitbox, const Vec2& velocity, Axis::Type axis) const {
	int min_x, max_x, min_y, max_y;

	min_x = ceilf(hitbox.position.x / tile_size.x);
	min_y = ceilf(hitbox.position.y / tile_size.y);
	max_x = floorf((hitbox.position.x + hitbox.size.x) / tile_size.x);
	max_y = floorf((hitbox.position.y + hitbox.size.y) / tile_size.y);

	if(axis == Axis::X) {
		if(velocity.x > 0.0f) {
			hitbox.position.x = (max_x) * tile_size.x - hitbox.size.x - EPS;  
		}
		else {
			hitbox.position.x = min_x * tile_size.x + EPS;
		}
	}
	else {
		if(velocity.y > 0.0f) {
			hitbox.position.y = (max_y) * tile_size.y - hitbox.size.y - EPS;  
		}
		else {
			hitbox.position.y = min_y * tile_size.y + EPS;
		}
	}
}

void World::setTile(int i, int j, int layer, TileId id) {
	if(outOfBounds(i, j, layer))
		return;

	tiles[layer][i + j * width] = id;
}

bool World::outOfBounds(int i, int j, int layer) const {
	return (i < 0) || (j < 0) || (i >= width) || (j >= height) || (layer < 0) || (layer > MAX_LAYERS); 
}
