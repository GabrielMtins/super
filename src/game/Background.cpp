#include "game/CustomEntities.hpp"

namespace Background {
	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_BACKGROUND;
	}

	static void renderBackground(Game *game, const std::string& name, float velocity) {
		Sprite background;

		background.setTexture(game->getTexture(name));
		background.ignore_camera = true;
		background.layer = RENDERLAYER_BACKGROUND;
		background.position.x = -game->getCameraPosition().x * velocity;

		for(int i = 0; i < 2; i++) {
			while(background.position.x + game->getScreenDimensions().x < 0.0f) {
				background.position.x += game->getScreenDimensions().x;
			}
			game->addSpriteToRenderList(background);

			background.position.x += game->getScreenDimensions().x;
		}
	}

	static void backgroundCity(Game *game) {
		renderBackground(game, "background-city", 0.5f);
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		backgroundCity(game);
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Background_GetHandler(void) {
	using namespace Background;
	return EntityHandler(create, update, collision);
}
