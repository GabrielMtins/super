#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <string>

#include "core/Context.hpp"
#include "core/Texture.hpp"

class ResourceManager {
	public:
		ResourceManager(void);
		bool load(Context *context, const std::string& filename);
		Texture * getTexture(const std::string& name);
		void quit(void);

	private:
		std::unordered_map<std::string, Texture> textures;

		void insertTexture(const Texture& texture);
};

#endif
