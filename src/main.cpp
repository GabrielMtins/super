#include <cstdio>
#include <istream>

#include "engine/Game.hpp"
#include "game/CustomEntities.hpp"
#include "core/TextGenerator.hpp"
#include "game/Constants.hpp"
#include "game/Scenes.hpp"

#include "nlohmann/json.hpp"

/*
 * TODO:
 * Adicionar leitor de configuração via json
 * Refazer o sistema de geração de textura de textos
 * Fazer um sistema de hud melhorado (não depender de várias entidades, adicionar sprites individualmente)
 *
 * DONE:
 * Programar sistema de knockback no player
 * Adicionar one way collisions
 * Adicionar interface de input no game
 * Adicionar colisão de mundo
 * Adicionar interface sfx
 * Adicionar interface de música
 * Adicionar leitor de recursos (texturas, sfx etc) utilizando arquivos json
 * Adicionar interfaces no Game para obter recursos
 * Adicionar sistema de câmera no game
 * Terminar renderização de mundo
 * Adicionar handler de colisão e de trigger
 * Adicionar sistemas de sprites (renderização)
 * Adicionar sprite renderer a parte
 * Adicionar gerador de textos para chaves dadas
 * Adicionar modo de pause
 * Fazer input de mouse e suas coordenadas no game
 * Ajeitar configurações de renderização
 * Mover as definições para um único hpp
 * Adicionar leitor de inimigos para as fases
 * Fazer sistema de animações
 *
 * Divisão de pastas explicadas:
 * - core: pasta utilizada para o core da engine, que pode ser reutilizado
 *   para outros projetos, como interfaces para as estruturas básicas do SDL2,
 *   interfaces para carregamento de arquivos etc.
 * - engine: pasta utilizada para a engine de fato, onde deverão estar mais
 *   ou menos as partes generalizadas, sistemas de colisão, update, renderização
 *   etc.
 * - game: pasta onde está o código do jogo de fato. Por exemplo, código de
 *   inimigos, player, cenas customizadas, leitura de arquivos, definição
 *   de tamanho da tela etc.
 */

static bool loadObject(Game *game, const nlohmann::json& object);
void loadNextScene(Game *game);

int main(int argc, char **argv) {
	Game *game;

	(void) argc;
	(void) argv;

	game = new Game();

	game->init("super", 240, 135, 8);
	game->setBackgroundColor(0xe0, 0xf8, 0xd0);
	game->setFps(165);
	game->setMinFps(50);
	game->loadRes("res/res.json");

	game->loadFontData("res/locale.json");
	game->loadLocale("res/locale.json", "pt-br");

	game->setWorldRenderLayerBg(RENDERLAYER_WORLD_BACKGROUND);
	game->setWorldRenderLayerFg(RENDERLAYER_WORLD_FOREGROUND);

	game->setInputKey(InputType::LEFT, SDL_SCANCODE_A);
	game->setInputKey(InputType::RIGHT, SDL_SCANCODE_D);
	game->setInputKey(InputType::JUMP, SDL_SCANCODE_K);
	game->setInputKey(InputType::FIRE, SDL_SCANCODE_J);
	game->setInputKey(InputType::START, SDL_SCANCODE_RETURN);

	game->setInputButton(InputType::LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	game->setInputButton(InputType::RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	game->setInputButton(InputType::JUMP, SDL_CONTROLLER_BUTTON_A);
	game->setInputButton(InputType::FIRE, SDL_CONTROLLER_BUTTON_X);
	game->setInputButton(InputType::START, SDL_CONTROLLER_BUTTON_START);

	Custom_AddEntityTypes(game);

	World *world = game->getWorld();
	world->setLoadObjectCallback(loadObject);
	world->setTexture(game->getTexture("world_tilemap"));
	world->setCollisionLayer(COLLISIONLAYER_STATIC);

	//game->loadScene(loadNextScene);
	game->loadScene(Scene::levelScreen);

	game->run();

	game->quit();

	printf("Game size: %lu KB\n", sizeof(Game) / 1024);
	printf("Entity size: %lu B\n", sizeof(Entity));

	delete game;

	return 0;
}

static bool loadObject(Game *game, const nlohmann::json& object) {
	EntityType type = 0;

	if(object.contains("type")) {
		std::string str = object.at("type");
		if(!str.empty()) {
			Entity *entity = game->getEntityFromId(game->addEntity(ENTITY_PLATFORM));
			entity->hitbox.position.x = object.at("x");
			entity->hitbox.position.y = object.at("y");
			entity->hitbox.size.x = object.at("width");
			entity->hitbox.size.y = object.at("height");
		}
	}

	if(!object.contains("gid"))
		return false;

	switch((int) object["gid"]) {
		case 257:
			type = ENTITY_WALKER;
			break;

		case 258:
			type = ENTITY_ITEMBOX;
			break;

		case 259:
			type = ENTITY_BALL;
			break;
	}

	Entity *entity = game->getEntityFromId(game->addEntity(type));

	if(object.contains("x")) {
		entity->hitbox.position.x = object["x"];
	}

	if(object.contains("y")) {
		entity->hitbox.position.y = object["y"];
	}

	return true;
}

void loadNextScene(Game *game) {
	game->loadWorld("res/levels/map01.tmj");

	game->addEntity(ENTITY_PLAYER);
	game->addEntity(ENTITY_CAMERA);
	game->addEntity(ENTITY_BACKGROUND);
	game->addEntity(ENTITY_HUD);
	game->addEntity(ENTITY_ITEMBOX);
	game->addEntity(ENTITY_DOOR);

	for(int i = 0; i < 2; i++) {
		Entity *entity = game->getEntityFromId(game->addEntity(ENTITY_WALKER));

		entity->hitbox.position.x += 16 * i + 80;
	}
}
