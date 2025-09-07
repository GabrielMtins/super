#include <cstdio>

#include "engine/Game.hpp"
#include "game/CustomEntities.hpp"

/*
 * TODO:
 * Adicionar interface de input no contexto
 * Adicionar interface de efeitos sonoros
 * Adicionar interface de música
 * Adicionar leitor de configuração via json # parcialmente feito
 * Adicionar leitor de recursos (texturas, sfx etc) utilizando arquivos json
 * Terminar renderização de mundo
 * Adicionar colisão de mundo
 */

int main(int argc, char **argv) {
	Game *game;

	(void) argc;
	(void) argv;

	game = new Game();
	game->loadRes("res/res.json");

	game->world.load("res/map01.tmj");
	game->world.setTexture(game->getTexture("world_tilemap"));
	game->world.setCollisionLayer(1);

	EntityList *entity_list = game->getEntityList();
	entity_list->addHandlerToType(0, Player_GetHandler());
	entity_list->addEntity(game, 0);

	game->run();

	delete game;

	return 0;
}
