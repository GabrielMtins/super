#include "game/CustomEntities.hpp"

namespace Player {
	static const float max_speed = 160.0f;
	static const float acceleration = 1600.0f;
	static const float friction = 800.0f;
	static const float min_speed = 10.0f;

	static const int CROSSHAIR_CHILD = 0;

	static void create(Game *game, Entity *entity) {
		(void) game;
		(void) entity;

		entity->sprite.setTexture(game->getTexture("character"));

		entity->size = Vec2(20.0f, 20.0f);
		entity->sprite.offset = Vec2(2.0f, 4.0f);
		entity->position = Vec2(200.0f, 100.0f);

		entity->collision_layer |= COLLISIONLAYER_PLAYER;
		entity->collision_mask |= COLLISIONLAYER_STATIC;
		entity->damage_cooldown = 200;

		//printf("%u\n", entity->collision_mask);
	}

	static Vec2 handleInput(Game *game, Entity *entity) {
		Vec2 direction;

		if(entity->children[CROSSHAIR_CHILD] == -1) {
			entity->children[CROSSHAIR_CHILD] = game->addEntity(ENTITY_CROSSHAIR);
		}

		if(game->getKey(Game::INPUT_LEFT)) {
			direction.x -= 1.0f;
		}

		if(game->getKey(Game::INPUT_RIGHT)) {
			direction.x += 1.0f;
		}

		if(game->getKey(Game::INPUT_UP)) {
			direction.y -= 1.0f;
		}

		if(game->getKey(Game::INPUT_DOWN)) {
			direction.y += 1.0f;
		}

		if(game->getMouseButtonDown(Game::MOUSEBUTTON_LEFT)) {
			Entity *bullet = game->getEntityFromId(game->addEntity(ENTITY_BULLET));
			Entity *crosshair = game->getEntityFromId(entity->children[CROSSHAIR_CHILD]);

			bullet->position = entity->position + entity->size / 2;

			bullet->velocity = (crosshair->position - bullet->position).normalized() * 400.0f;
		}

		if(direction.lengthSqr() > 1.0f) {
			direction = direction.normalized();
		}

		return direction;

		(void) entity;
	}

	static void handlePhysics(Game *game, Entity *entity, float dt, const Vec2& new_direction) {
		Vec2 current_direction;

		if(new_direction.lengthSqr() == 0.0f) {
			if(entity->velocity.lengthSqr() < min_speed * min_speed) {
				entity->velocity = Vec2::zero;
			} else {
				current_direction = entity->velocity.normalized();
				entity->velocity += current_direction * (-friction) * dt;
			}
		}

		entity->velocity += new_direction * acceleration * dt;

		if(entity->velocity.lengthSqr() > max_speed * max_speed) {
			entity->velocity = entity->velocity.normalized() * max_speed;
		}

		//entity->velocity = direction * speed;
		(void) game;
	}

	static void update(Game *game, Entity *entity, float dt) {
		handlePhysics(
				game,
				entity,
				dt,
				handleInput(game, entity)
				);

		game->setCameraPosition(
				entity->position - game->getScreenDimensions() * 0.5f
				);

		//entity->velocity = game->getMouseMotion() * 1000 / dt;
		//entity->position += game->getMouseMotion() * 800;

		(void) game;
		(void) entity;
		(void) dt;
	}

	static void collision(Game *game, Entity *entity, Entity *other) {
		(void) game;
		(void) entity;
		(void) other;
	}
}

EntityHandler Player_GetHandler(void) {
	using namespace Player;
	return EntityHandler(create, update, collision);
}
