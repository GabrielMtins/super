#include "core/ResourceManager.hpp"
#include "core/Console.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

ResourceManager::ResourceManager(void) {
}

bool ResourceManager::load(Context *context, const std::string& filename) {
	std::ifstream f(filename);
	json data;

	if(!f.is_open()) {
		console.error((std::string) "[Resource Manager] File doesn't exist: " + filename);
		console.exit(1);
	}

	console.log((std::string) "[Resource Manager] Opened resource file: " + filename);

	/* check if the data can be parsed */
	try {
		data = json::parse(f);
		console.log((std::string) "[Resource Manager] Parsed file successfully: " + filename);
	} catch(const json::exception& ex) {
		console.error((std::string) "[Resource Manager] Error parsing file " + filename + ": " + (std::string) ex.what());
		console.exit(1);
		return false;
	}

	/* load textures */
	try {
		json& json_textures = data["textures"];

		for(json& it : json_textures) {
			jsonToTexture(it, context);
		}
	} catch(const json::exception& ex) {
		console.error("[Resource Manager] " + (std::string) ex.what());
		console.exit(1);
	}

	/* load sfxs */
	try {
		json& json_sfxs = data["sfxs"];

		for(json& it : json_sfxs) {
			jsonToSfx(it);
		}
	} catch(const json::exception& ex) {
		console.error("[Resource Manager] " + (std::string) ex.what());
		console.exit(1);
	}

	/* load musics */
	try {
		json& json_musics = data["musics"];

		for(json& it : json_musics) {
			jsonToMusic(it);
		}
	} catch(const json::exception& ex) {
		console.error("[Resource Manager] " + (std::string) ex.what());
		console.exit(1);
	}

	/* load level paths */
	try {
		std::vector<std::string> vec_level_paths = data["levels"];

		if(vec_level_paths.size() > MAX_LEVEL_PATHS) {
			console.error("[Resource Manager] The maximum number of level paths is 16.");
			console.exit(1);
		}

		std::copy_n(vec_level_paths.begin(), vec_level_paths.size(), level_paths.begin());
		num_levels = vec_level_paths.size();

		console.log("[Resource Manager] Loaded level paths successfully!");
	} catch(const json::exception& ex) {
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

Sfx * ResourceManager::getSfx(const std::string& name) {
	if(sfxs.find(name) == sfxs.end())
		return NULL;

	return &(sfxs.at(name));
}

Music * ResourceManager::getMusic(const std::string& name) {
	if(musics.find(name) == musics.end())
		return NULL;

	return &(musics.at(name));
}

void ResourceManager::quit(void) {
	for(auto& [_, texture] : textures) {
		texture.unload();
	}

	for(auto& [_, sfx] : sfxs) {
		sfx.unload();
	}

	for(auto& [_, music] : musics) {
		music.unload();
	}
}

void ResourceManager::jsonToTexture(const nlohmann::json& j, Context *context) {
	const std::string& name = j.at("name");

	if(textures.find(name) != textures.end()) {
		console.error("[Resource Manager] There is already a texture with name: " + name);
		console.exit(1);
	}

	Texture& texture = textures[name];

	texture.load(context, j.at("path"));
	texture.setCellSize(
			j.at("cell_width"),
			j.at("cell_height")
			);
}

void ResourceManager::jsonToSfx(const nlohmann::json& j) {
	const std::string& name = j.at("name");

	if(sfxs.find(name) != sfxs.end()) {
		console.error("[Resource Manager] There is already a sfx with name: " + name);
		console.exit(1);
	}

	Sfx& sfx = sfxs[name];

	sfx.load(j.at("path"));
}

void ResourceManager::jsonToMusic(const nlohmann::json& j) {
	const std::string& name = j.at("name");

	if(musics.find(name) != musics.end()) {
		console.error("[Resource Manager] There is already a music with name: " + name);
		console.exit(1);
	}

	Music& music = musics[name];

	music.load(j.at("path"));
}
