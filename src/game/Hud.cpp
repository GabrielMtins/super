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
		sprite.hud_element = true;
		sprite.ignore_camera = true;

		for(int i = 1; i < player->health; i++) {
			sprite.cell = 1;
			game->addSpriteToRenderList(sprite);

			sprite.position.x += 9.0f;
		}

		for(int i = player->health; i < PLAYER_MAX_HEALTH; i++) {
			sprite.cell = 4;
			game->addSpriteToRenderList(sprite);

			sprite.position.x += 9.0f;
		}
	}

	static void drawHud(Game *game, Entity *entity) {
		Entity *player;

		player = game->getEntityFromId(entity->target_id);

		if(player == NULL)
			return;

		drawPlayerLifebar(game, player);
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
