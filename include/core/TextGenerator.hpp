#ifndef TEXTGENERATOR_HPP
#define TEXTGENERATOR_HPP

#include "core/Context.hpp"
#include "core/Texture.hpp"

#include <unordered_map>

#include "nlohmann/json_fwd.hpp"

/**
 * Classe que lê um arquivo locale e gera texturas de texto
 * dado strings. Útil para computar as texturas de texto no início
 * do programa, em vez de fazer isso todo frame ou durante o runtime.
 */
class TextGenerator {
	public:
		TextGenerator(void);

		/** 
		 * Carrega uma fonte dado o seu diretório e seu tamanho.
		 */
		bool loadFontData(const std::string& filename);

		/**
		 * Carrega o arquivo de locale, um arquivo json contendo as strings
		 * de texto utilizada pelo jogo. 
		 */
		bool loadLocale(Context *context, const std::string& filename, const std::string& lang);

		/**
		 * Retorna uma textura dado o seu nome (chave), que foi
		 * escrito no arquivo de locale.
		 */
		Texture * getTexture(const std::string& name);

		/**
		 * Descarrega da memória as texturas geradas pelo locale e também
		 * a fonte.
		 */
		void quit(void);

	private:
		bool loadFont(const nlohmann::json& data);

		std::string path;

		std::unordered_map<std::string, TTF_Font *> fonts;
		std::unordered_map<std::string, Texture> textures;

};

#endif
