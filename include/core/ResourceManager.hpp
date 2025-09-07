#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <string>
#include <array>

#include "core/Context.hpp"
#include "core/Texture.hpp"

#include "nlohmann/json_fwd.hpp"

#define MAX_LEVEL_PATHS 16

class ResourceManager {
	public:
		ResourceManager(void);
		bool load(Context *context, const std::string& filename);
		Texture * getTexture(const std::string& name);
		void quit(void);

	private:
		void jsonToTexture(const nlohmann::json& j, Context *context);

		std::unordered_map<std::string, Texture> textures;
		std::array<std::string, MAX_LEVEL_PATHS> level_paths;
		size_t num_levels;
};

#endif
