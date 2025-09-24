#include <cstdio>

#include "engine/Game.hpp"
#include "game/CustomEntities.hpp"
#include "core/TextGenerator.hpp"

#include "nlohmann/json.hpp"

/*
 * TODO:
 * Adicionar leitor de configuração via json
 * Adicionar one way collisions
 * Refazer o sistema de geração de textura de textos
 *
 * DONE:
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

int main(int argc, char **argv) {
	Game *game;

	(void) argc;
	(void) argv;

	game = new Game();

	game->init("super", 240, 135);
	game->loadRes("res/res.json");

	game->loadFont("res/PublicPixel.ttf", 8);
	game->loadLocale("res/locale.json", "pt-br");

	Custom_AddEntityTypes(game);

	World *world = game->getWorld();
	world->setLoadObjectCallback(loadObject);
	world->setTexture(game->getTexture("world_tilemap"));
	world->setCollisionLayer(COLLISIONLAYER_STATIC);

	game->loadWorld("res/levels/map01.tmj");

	game->addEntity(ENTITY_PLAYER);

	for(int i = 0; i < 16; i++) {
		Entity *entity = game->getEntityFromId(game->addEntity(ENTITY_ZOMBIE));

		entity->hitbox.position.x += i * 16;
	}

	game->run();

	game->quit();

	delete game;

	return 0;
}

static bool loadObject(Game *game, const nlohmann::json& object) {
	EntityType type;

	if(!object.contains("gid"))
		return false;

	switch((int) object["gid"]) {
		case 257:
			type = ENTITY_ZOMBIE;
			return false;
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
