#include <cstdio>

#include "engine/Game.hpp"
#include "game/CustomEntities.hpp"

/*
 * TODO:
 * Adicionar leitor de configuração via json
 * Adicionar one way collisions
 * Adicionar handler de colisão e de trigger
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
 */

int main(int argc, char **argv) {
	Game *game;

	(void) argc;
	(void) argv;

	game = new Game();

	game->init("super", 480, 270);
	game->loadRes("res/res.json");

	World *world = game->getWorld();

	world->load(game->getContext(), "res/map01.tmj");
	world->setTexture(game->getTexture("world_tilemap"));
	world->setCollisionLayer(1);

	EntityList *entity_list = game->getEntityList();
	entity_list->addHandlerToType(0, Player_GetHandler());
	entity_list->addEntity(game, 0);

	Entity *entity = entity_list->getEntityFromId(entity_list->addEntity(game, 1));
	entity->texture = game->getTexture("character");
	//entity->hud_element = true;

	game->run();

	game->quit();

	delete game;

	return 0;
}
