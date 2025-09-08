#include "core/TextGenerator.hpp"
#include "core/Console.hpp"

#include "nlohmann/json.hpp"

#include <fstream>

using json = nlohmann::json;

static void from_json(const json& j, SDL_Color &color);

TextGenerator::TextGenerator(void) {
	font = NULL;
}

bool TextGenerator::loadFont(const std::string& filename, int size) {
	font = TTF_OpenFont(filename.c_str(), size);
	
	if(font == NULL) {
		console.error("[Text Generator] Failed to open file: " + filename);
		return false;
	}

	console.log("[Text Generator] Loaded font: " + filename);

	path = filename;

	return true;
}

bool TextGenerator::loadLocale(Context *context, const std::string& filename, const std::string& lang) {
	std::ifstream file;
	json data;

	if(font == NULL) {
		console.error("[Text Generator] There is no font loaded!");
		return false;
	}

	file.open(filename);

	if(!file.is_open()) {
		console.error("[Text Generator] Failed to open locale file: " + filename);
		return false;
	}

	try {
		data = json::parse(file);
	} catch(const json::exception& ex) {
		console.error((std::string) "[Text Generator] " + ex.what());
		return false;
	}

	try {
		json& locale = data["languages"][lang];
		SDL_Color color = data["color"];

		for(auto& el : locale.items()) {
			textures[el.key()].generateText(context, font, el.value(), color);
		}
	} catch(const json::exception& ex) {
		console.error((std::string) "[Text Generator] " + ex.what());
		return false;
	}

	return true;
}

Texture * TextGenerator::getTexture(const std::string& name) {
	if(textures.find(name) == textures.end())
		return NULL;

	return &textures[name];
}

void TextGenerator::quit(void) {
	if(font != NULL) {
		TTF_CloseFont(font);
		console.log("[Text Generator] Unloading font: " + path);
	}

	for(auto& [_, texture] : textures) {
		texture.unload();
	}
}

static void from_json(const json& j, SDL_Color &color) {
	uint32_t color_index = std::stoi((std::string) j, nullptr, 16);

	color.r = ((color_index & 0xff0000) >> 16);
	color.g = ((color_index & 0x00ff00) >> 8);
	color.b = ((color_index & 0x0000ff));
	color.a = 0xff;
}
