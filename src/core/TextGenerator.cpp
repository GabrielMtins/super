#include "core/TextGenerator.hpp"
#include "core/Console.hpp"

#include "nlohmann/json.hpp"

#include <fstream>

using json = nlohmann::json;

static void from_json(const json& j, SDL_Color &color);

TextGenerator::TextGenerator(void) {
}

bool TextGenerator::loadFontData(const std::string& filename) {
	std::ifstream file;

	json data;

	file.open(filename);

	if(!file.is_open()) {
		console.error("[Text Generator] Failed to open file: " + filename);
		return false;
	}

	try {
		data = json::parse(file);
	} catch(const json::exception& ex) {
		console.error((std::string) "[Text Generator] " + ex.what());
		return false;
	}

	try {
		const json& fonts_json = data.at("fonts");

		for(const json& font_data : fonts_json) {
			loadFont(font_data);
		}
	} catch(const json::exception& ex) {
		console.error((std::string) "[Text Generator] " + ex.what());
		return false;
	}

	return true;
}

bool TextGenerator::loadLocale(Context *context, const std::string& filename, const std::string& lang) {
	std::ifstream file;
	json data;

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
		const json& keys_data = data["keys"];

		for(const auto& el : keys_data.items()) {
			const std::string& key_name = el.key();
			const std::string& font_name = el.value().at("font");
			SDL_Color color = el.value().at("color");
			const std::string& text = data["languages"][lang][key_name];

			if(fonts.find(font_name) == fonts.end())
				continue;

			textures[key_name].generateText(
					context,
					fonts.at(font_name),
					text,
					color
					);
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
	for(auto& [_, font] : fonts) {
		TTF_CloseFont(font);
	}

	for(auto& [_, texture] : textures) {
		texture.unload();
	}
}

bool TextGenerator::loadFont(const json& data) {
	const std::string& path = data.at("path");
	const std::string& name = data.at("name");
	int size = data.at("size");

	TTF_Font *font = TTF_OpenFont(path.c_str(), size);

	if(font == NULL)
		return false;

	fonts[name] = font;

	return true;
}

static void from_json(const json& j, SDL_Color &color) {
	uint32_t color_index = std::stoi((std::string) j, nullptr, 16);

	color.r = ((color_index & 0xff0000) >> 16);
	color.g = ((color_index & 0x00ff00) >> 8);
	color.b = ((color_index & 0x0000ff));
	color.a = 0xff;
}

