#include "engine/EntityList.hpp"
#include "engine/Game.hpp"

EntityList::EntityList(void) {
	clearEntities();
}

void EntityList::update(Game *game, float dt) {
	for(size_t i = 0; i < num_entities; i++) {
		Entity& entity = entities[i];

		if(!shouldProcessEntity(game, entity))
			continue;

		entity.hitbox.position.y += (entity.velocity.y + entity.contact_velocity.y) * dt;
		solveEntityCollisionWithWorld(game, game->getWorld(), entity, Axis::Y);

		entity.hitbox.position.x += (entity.velocity.x + entity.contact_velocity.x) * dt;
		solveEntityCollisionWithWorld(game, game->getWorld(), entity, Axis::X);

		findAndSolveEntityCollisions(game, game->getWorld(), entity);

		entity.updateCenter();
	}

	for(size_t i = 0; i < num_entities; i++) {
		Entity& entity = entities[i];
		EntityHandler& handler = type_to_handler[entity.type];

		if(handler.update != NULL)
			handler.update(game, &entity, dt);

		entity.updateAnimator(game);
		entity.updateSprite();
	}

	cleanUp();
}

void EntityList::setSpriteRenderList(Game *game, SpriteRenderer* sprite_renderer) {
	for(size_t i = 0; i < num_entities; i++) {
		sprite_renderer->addSpriteToRenderList(game, entities[i].sprite);
	}
}

void EntityList::clearEntities(void) {
	num_entities = 0;
	next_id = 1;
	id_to_entity.clear();
}

Entity * EntityList::getEntityFromId(EntityId id) {
	if(id_to_entity.find(id) == id_to_entity.end())
		return NULL;

	return id_to_entity[id];
}

EntityId EntityList::addEntity(Game *game, EntityType type) {
	EntityId new_id = next_id; 
	Entity *new_entity;
	EntityHandler& handler = type_to_handler[type];

	new_entity = &entities[num_entities];
	id_to_entity[next_id++] = new_entity;

	*new_entity = Entity(new_id);
	new_entity->type = type;
	new_entity->alive = true;

	if(handler.create != NULL)
		handler.create(game, new_entity);

	num_entities++;

	return new_id;
}

void EntityList::addHandlerToType(EntityType type, const EntityHandler& handler) {
	if(type < 0 || type >= MAX_ENTITY_TYPES)
		return;

	type_to_handler[type] = handler;
}

EntityFoundList EntityList::findEntity(EntityType type) {
	found_entities.clear();

	for(size_t i = 0; i < num_entities; i++) {
		const Entity& entity = entities[i];

		if(entity.type == type)
			found_entities.push_back(entity.getId());
	}

	return found_entities;
}

EntityFoundList EntityList::findEntity(const Vec2& position, float radius) {
	found_entities.clear();

	for(size_t i = 0; i < num_entities; i++) {
		const Entity& entity = entities[i];

		if((entity.center - position).lengthSqr() < radius * radius)
			found_entities.push_back(entity.getId());
	}

	return found_entities;
}

EntityFoundList EntityList::findEntity(EntityType type, const Vec2& position, float radius) {
	found_entities.clear();

	for(size_t i = 0; i < num_entities; i++) {
		const Entity& entity = entities[i];

		if(entity.type != type)
			continue;

		if((entity.center - position).lengthSqr() > radius * radius)
			continue;

		found_entities.push_back(entity.getId());
	}

	return found_entities;
}

bool EntityList::checkCollision(const World *world, const Hitbox& hitbox) const {
	if(world->checkCollision(hitbox))
		return true;

	for(size_t i = 0; i < num_entities; i++) {
		const Entity& entity = entities[i];

		if(hitbox.checkCollision(entity.hitbox))
			return true;
	}

	return false;
}

void EntityList::findAndSolveEntityCollisions(Game *game, const World *world, Entity& entity) {
	EntityHandler& handler = type_to_handler[entity.type];

	for(size_t i = 0; i < num_entities; i++) {
		Entity& other = entities[i];
		Vec2 old_position;
		Vec2 old_velocity;
		EntityHandler& other_handler = type_to_handler[other.type];

		old_position = entity.hitbox.position;
		old_velocity = entity.velocity;

		if(entity.getId() == other.getId())
			continue;

		if(!entity.hitbox.checkCollision(other.hitbox, entity.velocity))
			continue;

		entity.hitbox.solveCollision(other.hitbox, entity.velocity);

		if(world->checkCollision(entity.hitbox)) {
			entity.hitbox.position = old_position;
			entity.velocity = old_velocity;

			if(!other.hitbox.checkCollision(entity.hitbox, other.velocity))
				continue;

			old_position = other.hitbox.position;
			old_velocity = other.velocity;

			other.hitbox.solveCollision(entity.hitbox, other.velocity);

			if(world->checkCollision(other.hitbox)) {
				other.hitbox.position = old_position;
				other.velocity = old_velocity;
			}
		}

		if(handler.collision != NULL) {
			handler.collision(game, &entity, &other);
		}

		if(other_handler.collision != NULL) {
			other_handler.collision(game, &other, &entity);
		}
	}
}

void EntityList::solveEntityCollisionWithWorld(Game *game, const World *world, Entity& entity, Axis::Type axis) {
	EntityHandler& handler = type_to_handler[entity.type];

	if(!world->checkCollision(entity.hitbox))
		return;

	world->solveCollision(entity.hitbox, entity.velocity + entity.contact_velocity, axis);

	if(axis == Axis::X) {
		entity.velocity.x = 0.0f;
		entity.contact_velocity.x = 0.0f;
	} else {
		entity.velocity.y = 0.0f;
		entity.contact_velocity.y = 0.0f;
	}

	if(handler.collision != NULL)
		handler.collision(game, &entity, NULL);
}

bool EntityList::shouldProcessEntity(const Game *game, const Entity& entity) {
	if(game->isPaused()){
		if(entity.pause_mode == Entity::PAUSEMODE_PAUSABLE)
			return false;
	} else {
		if(entity.pause_mode == Entity::PAUSEMODE_WHENPAUSED)
			return false;
	}

	return true;
}

void EntityList::cleanUp(void) {
	for(size_t i = 0; i < num_entities; i++) {
		if(!entities[i].alive) {
			removeEntity(i);
		}
	}
}

void EntityList::removeEntity(int position) {
	if(num_entities == 0)
		return;

	id_to_entity.erase(entities[position].getId());

	if(num_entities > 1) {
		entities[position] = entities[num_entities - 1];
		id_to_entity[entities[position].getId()] = &entities[position];
	}

	num_entities--;
}
