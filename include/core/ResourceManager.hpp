#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <string>
#include <array>

#include "core/Context.hpp"
#include "core/Texture.hpp"
#include "core/Sfx.hpp"
#include "core/Music.hpp"
#include "core/CoreDef.hpp"

#include "nlohmann/json_fwd.hpp"

/**
 * Classe que lê um arquivo json com os materiais utilizados no jogo,
 * como efeitos sonoros, músicas, fases e texturas.
 */
class ResourceManager {
	public:
		ResourceManager(void);
		bool load(Context *context, const std::string& filename);

		/**
		 * Retorna uma Texture * dado o seu nome. Caso não haja nenhuma
		 * textura com esse nome, retorna NULL.
		 */
		Texture * getTexture(const std::string& name);

		/**
		 * Retorna um Sfx * dado o seu nome. Caso não haja nenhum
		 * Sfx com esse nome, retorna NULL.
		 */
		Sfx * getSfx(const std::string& name);

		/**
		 * Retorna um Music * dado o seu nome. Caso não haja nenhum
		 * Music com esse nome, retorna NULL.
		 */
		Music * getMusic(const std::string& name);

		/**
		 * Descarrega os recursos da memória.
		 */
		void quit(void);

	private:
		void jsonToTexture(const nlohmann::json& j, Context *context);
		void jsonToSfx(const nlohmann::json& j);
		void jsonToMusic(const nlohmann::json& j);

		std::unordered_map<std::string, Texture> textures;
		std::unordered_map<std::string, Sfx> sfxs;
		std::unordered_map<std::string, Music> musics;

		std::array<std::string, MAX_LEVEL_PATHS> level_paths;
		size_t num_levels;
};

#endif
