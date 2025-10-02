#include "game/CustomEntities.hpp"
#include "game/Player.hpp"

namespace Hud {
	enum HudStates {
		STATE_START,
		STATE_RUNNING
	};

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->type = ENTITY_HUD;
		entity->state = STATE_START;
	}

	static void drawPlayerLifebar(Game *game, Entity *player) {
		Sprite sprite;

		sprite.setTexture(game->getTexture("8x8-items"));
		sprite.position = Vec2(2.0f, 2.0f);
		sprite.layer = RENDERLAYER_HUD;
		sprite.ignore_camera = true;

		for(int i = 1; i < player->health; i++) {
			static const int heart_cell = 1;
			sprite.cell = heart_cell;
			game->addSpriteToRenderList(sprite);

			sprite.position.x += 9.0f;
		}

		for(int i = player->health; i < PLAYER_MAX_HEALTH; i++) {
			static const int hollow_heart_cell = 4;
			sprite.cell = hollow_heart_cell;
			game->addSpriteToRenderList(sprite);

			sprite.position.x += 9.0f;
		}
	}

	static void drawPlayerKey(Game *game, Entity *player) {
		if(!Player::hasKey(player))
			return;

		Sprite sprite;

		sprite.layer = RENDERLAYER_HUD;
		sprite.ignore_camera = true;

		sprite.setTexture(game->getTexture("8x8-items"));
		sprite.position = Vec2(
				game->getScreenDimensions().x - 16.0f,
				5.0f
				);


		static const int key_cell = 5;

		sprite.cell = key_cell;

		game->addSpriteToRenderList(sprite);
	}

	static void drawHud(Game *game, Entity *entity) {
		Entity *player;

		player = game->getEntityFromId(entity->target_id);

		if(player == NULL)
			return;

		drawPlayerLifebar(game, player);
		drawPlayerKey(game, player);
	}

	static void update(Game *game, Entity *entity, float dt) {
		(void) game;
		(void) entity;
		(void) dt;

		switch(entity->state) {
			case STATE_START:
				{
					EntityFoundList list = game->findEntity(ENTITY_PLAYER);
					if(list.size() > 0) {
						entity->target_id = list.at(0);
						entity->state = STATE_RUNNING;
					}
				}
				break;

			case STATE_RUNNING:
				drawHud(game, entity);
				break;
		}
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Hud_GetHandler(void) {
	using namespace Hud;
	return EntityHandler(create, update, collision);
}
