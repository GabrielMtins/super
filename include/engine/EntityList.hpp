#ifndef ENTITYLIST_HPP
#define ENTITYLIST_HPP

#include "engine/Entity.hpp"
#include "engine/EntityHandler.hpp"
#include <array>
#include <unordered_map>

class Game;

#define MAX_ENTITIES 1024
#define MAX_ENTITY_TYPES 64

class EntityList {
	public:
		EntityList(void);
		void update(Game *game);
		void render(Game *game);
		void clearEntities(void);
		Entity * getEntityFromId(EntityId id);
		EntityId addEntity(Game *game, EntityType type);
		void addHandlerToType(EntityType type, const EntityHandler& handler);

	private:
		std::array<Entity, MAX_ENTITIES> entities;
		std::unordered_map<EntityId, Entity *> id_to_entity;
		std::array<EntityHandler, MAX_ENTITY_TYPES> type_to_handler;
		size_t num_entities;
		EntityId next_id;

		void removeEntity(int position);
};

#endif
