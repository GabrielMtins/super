#include "engine/World.hpp"

#include "core/Console.hpp"

#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

World::World(void) {
	width = 0;
	height = 0;
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
	} catch(const json::exception& ex) {
		console.error("[World] World file doesn't have width or height.");
		return false;
	}

	return true;
}
