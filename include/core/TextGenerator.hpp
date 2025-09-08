#ifndef TEXTGENERATOR_HPP
#define TEXTGENERATOR_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"

#include <unordered_map>

class TextGenerator {
	public:
		TextGenerator(void);

		bool loadFont(const std::string& filename, int size);

		bool loadLocale(Context *context, const std::string& filename, const std::string& lang);
		Texture * getTexture(const std::string& name);

		void quit(void);

	private:
		TTF_Font *font;
		std::string path;

		std::unordered_map<std::string, Texture> textures;
};

#endif
