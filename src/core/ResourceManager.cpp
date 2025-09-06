#include "core/ResourceManager.hpp"
#include "core/Console.hpp"

#include <fstream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

static Texture JsonToTexture(const json& j, Context *context) {
	Texture texture;

	texture.load(context, j.at("path"));
	texture.setName(j.at("name"));
	texture.setCellSize(
			j.at("cell_width"),
			j.at("cell_height")
			);

	return texture;
}

ResourceManager::ResourceManager(void) {
}

bool ResourceManager::load(Context *context, const std::string& filename) {
	std::ifstream f(filename);
	json data;

	/* check if the data can be parsed */
	try {
		data = json::parse(f);
		console.log((std::string) "[Resource Manager] Parsed file successfully: " + filename);
	} catch(const json::parse_error& ex) {
		console.error((std::string) "[Resource Manager] Error parsing file " + filename + ": " + (std::string) ex.what());
		console.exit(1);
		return false;
	}

	/* load textures */
	try {
		json& json_textures = data["textures"];

		for(json& it : json_textures) {
			insertTexture(JsonToTexture(it, context));
		}
	} catch(const json::out_of_range& ex) {
		console.error("[Resource Manager] " + (std::string) ex.what());
		console.exit(1);
	}

	return true;
}

Texture * ResourceManager::getTexture(const std::string& name) {
	if(textures.find(name) == textures.end())
		return NULL;

	return &(textures.at(name));
}

void ResourceManager::quit(void) {
	for(auto& [_, texture] : textures) {
		texture.unload();
	}
}

void ResourceManager::insertTexture(const Texture& texture) {
	textures[texture.getName()] = texture;
}
