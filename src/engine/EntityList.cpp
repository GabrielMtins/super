#include "engine/EntityList.hpp"
#include "engine/Game.hpp"

EntityList::EntityList(void) {
	clearEntities();
}

void EntityList::update(Game *game) {
	float dt = 0.001f;

	for(size_t i = 0; i < num_entities; i++) {
		Entity& entity = entities[i];
		EntityHandler& handler = type_to_handler[entity.type];

		entity.position.x += entity.velocity.x * dt;
		findAndSolveEntityCollisions(entity, Entity::AXIS_X);

		entity.position.y += entity.velocity.y * dt;
		findAndSolveEntityCollisions(entity, Entity::AXIS_Y);

		if(handler.update == NULL)
			continue;

		handler.update(game, &entity, 0.001f);
	}
}

void EntityList::render(Game *game) {
	for(size_t i = 0; i < num_entities; i++) {
		Entity& entity = entities[i];

		entity.render(game);
	}
}

void EntityList::clearEntities(void) {
	num_entities = 0;
	next_id = 0;
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

	*new_entity = Entity();
	new_entity->id = new_id;
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

void EntityList::findAndSolveEntityCollisions(Entity& entity, Entity::Axis axis) {
	for(size_t i = 0; i < num_entities; i++) {
		Entity& other = entities[i];

		if(entity.id == other.id)
			continue;

		entity.solveCollision(other, axis);
	}
}

void EntityList::removeEntity(int position) {
	if(num_entities == 0)
		return;

	id_to_entity.erase(entities[position].id);

	if(num_entities > 1) {
		entities[position] = entities[num_entities - 1];
		id_to_entity[entities[position].id] = &entities[position];
	}

	num_entities--;
}
