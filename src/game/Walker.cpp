#include "game/CustomEntities.hpp"

#define DEFAULT_VELOCITY 40
#define CHANGE_STATE_FLAG 0

enum WalkerStates {
	STATE_LEFT = 0,
	STATE_RIGHT,
};

namespace Walker {
	static void create(Game *game, Entity *entity) {
		entity->sprite.setTexture(game->getTexture("walker"));
		entity->size = Vec2(16.0f, 16.0f);
		entity->sprite.offset = Vec2(4.0f, 8.0f);

		entity->collision_layer = 1;
		entity->collision_mask = 1;
		entity->state = STATE_LEFT;

		entity->position = Vec2(300.0f, 0.0f);
		entity->updateSprite();
	}

	static void changeState(Entity *entity) {
		entity->state = entity->state == STATE_LEFT ? STATE_RIGHT : STATE_LEFT;
	}

	static void update(Game *game, Entity *entity, float dt) {
		switch(entity->state) {
			case STATE_LEFT:
				entity->velocity.x = DEFAULT_VELOCITY;
				entity->sprite.flip_x = false;
				break;

			case STATE_RIGHT:
				entity->velocity.x = -DEFAULT_VELOCITY;
				entity->sprite.flip_x = true;
				break;
		}

		entity->velocity.y += 500.0f * dt;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		if(entity->velocity.x == 0.0f) {
			changeState(entity);

		}
	}
}

EntityHandler Walker_GetHandler(void) {
	using namespace Walker;
	return EntityHandler(create, update, collision);
}
