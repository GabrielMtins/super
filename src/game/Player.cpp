#include "game/CustomEntities.hpp"

namespace Player {
	static void create(Game *game, Entity *entity) {
		entity->sprite.setTexture(game->getTexture("character"));
		entity->collision_mask = 0;
		entity->size = Vec2(20.0f, 20.0f);
		entity->collision_layer = 1;
		entity->collision_mask = 1;
		entity->sprite.offset = Vec2(2.0f, 4.0f);

		entity->pause_mode = Entity::PAUSEMODE_WHENPAUSED;
	}

	static void update(Game *game, Entity *entity, float dt) {
		Vec2 camera_pos(
				entity->position.x - game->getScreenDimensions().x / 2,
				-32.0f
			   );

		if(camera_pos.x < 0.0f)
			camera_pos.x = 0.0f;

		game->setCameraPosition(camera_pos);
		entity->velocity.x = 0.0f;

		if(game->getKey(Game::INPUT_LEFT)) {
			entity->velocity.x += -150.0f;
			entity->sprite.flip_x = true;
		}

		if(game->getKey(Game::INPUT_RIGHT)) {
			entity->velocity.x += 150.0f;
			entity->sprite.flip_x = false;
		}

		if(game->getKey(Game::INPUT_DOWN)) {
			entity->velocity.y = 150.0f;
		}

		if(game->getKey(Game::INPUT_UP) && entity->velocity.y == 0.0f) {
			entity->velocity.y = -200.0f;
			entity->sprite.cell = 2;
		}


		if(game->getKeyDown(Game::INPUT_FIRE)) {
			game->pause();
		}

		entity->velocity.y += 200.0f * dt;

	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		if(entity->velocity.y == 0.0f)
			entity->sprite.cell = 0;

		if(other == NULL)
			return;

		if(other->type == ENTITY_WALKER) {
			other->alive = false;
		}
	}
};

EntityHandler Player_GetHandler(void) {
	return EntityHandler(Player::create, Player::update, Player::collision);
}
