#include <cstdio>

#include "engine/Game.hpp"
#include "game/CustomEntities.hpp"
#include "core/TextGenerator.hpp"

/*
 * TODO:
 * Adicionar leitor de configuração via json
 * Adicionar one way collisions
 * Adicionar leitor de inimigos para as fases
 * Fazer input de mouse e suas coordenadas no game
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

int main(int argc, char **argv) {
	Game *game;

	(void) argc;
	(void) argv;

	game = new Game();

	game->init("super", 480, 270);
	game->loadRes("res/res.json");

	game->loadFont("res/PublicPixel.ttf", 8);
	game->loadLocale("res/locale.json", "pt-br");

	World *world = game->getWorld();

	world->load(game->getContext(), "res/map01.tmj");
	world->setTexture(game->getTexture("world_tilemap"));
	world->setCollisionLayer(COLLISIONLAYER_STATIC);

	Custom_AddEntityTypes(game);

	game->addEntity(ENTITY_PLAYER);
	game->addEntity(ENTITY_ZOMBIE);

	game->run();

	game->quit();

	delete game;

	return 0;
}
