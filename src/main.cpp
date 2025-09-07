#include <cstdio>

#include "engine/Game.hpp"
#include "game/CustomEntities.hpp"

/*
 * TODO:
 * Adicionar leitor de configuração via json
 * Adicionar leitor de recursos (texturas, sfx etc) utilizando arquivos json
 * Terminar renderização de mundo
 * Adicionar one way collisions
 *
 * DONE:
 * Adicionar interface de input no game
 * Adicionar colisão de mundo
 * Adicionar interface sfx
 * Adicionar interface de música
 */

int main(int argc, char **argv) {
	Game *game;

	(void) argc;
	(void) argv;

	game = new Game();

	game->init("super", 480, 270);
	game->loadRes("res/res.json");

	World *world = game->getWorld();

	world->load("res/map01.tmj");
	world->setTexture(game->getTexture("world_tilemap"));
	world->setCollisionLayer(1);

	EntityList *entity_list = game->getEntityList();
	entity_list->addHandlerToType(0, Player_GetHandler());
	entity_list->addEntity(game, 0);

	game->run();

	game->quit();

	delete game;

	return 0;
}
