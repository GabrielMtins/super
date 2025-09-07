#include <cstdio>

#include "engine/Game.hpp"
#include "game/CustomEntities.hpp"

/*
 * TODO:
 * Adicionar interface de input no contexto
 * Adicionar interface de efeitos sonoros
 * Adicionar interface de música
 * Adicionar leitor de configuração via json+
 * Adicionar leitor de recursos (texturas, sfx etc) utilizando arquivos json
 */

int main(int argc, char **argv) {
	Game *game;

	(void) argc;
	(void) argv;

	game = new Game();

	EntityList *entity_list = game->getEntityList();
	entity_list->addHandlerToType(0, Player_GetHandler());
	entity_list->addEntity(game, 0);

	Entity *entity = entity_list->getEntityFromId(entity_list->addEntity(game, 1));
	entity->texture = game->getTexture("character");
	entity->position = Vec2(64.0f, 64.0f);
	entity->size = Vec2(32.0f, 32.0f);
	entity->collision_layer = 1;

	game->run();

	delete game;

	return 0;
}
