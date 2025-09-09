#ifndef ENTITYLIST_HPP
#define ENTITYLIST_HPP

#include "engine/Entity.hpp"
#include "engine/EntityHandler.hpp"
#include "engine/World.hpp"

#include "engine/SpriteRenderer.hpp"

#include <array>
#include <vector>
#include <unordered_map>

class Game;

#define MAX_ENTITIES 1024
#define MAX_ENTITY_TYPES 64

using EntityFoundList = std::vector<EntityId>;

class EntityList {
	public:
		EntityList(void);
		void update(Game *game, float dt);
		void setSpriteRenderList(Game *game, SpriteRenderer* sprite_renderer);
		void clearEntities(void);
		Entity * getEntityFromId(EntityId id);
		EntityId addEntity(Game *game, EntityType type);
		void addHandlerToType(EntityType type, const EntityHandler& handler);
		const EntityFoundList& findEntitiesByType(EntityType type);
		const EntityFoundList& findEntitiesByRadius(const Vec2& position, float radius);
		const EntityFoundList& findEntitiesByRadiusAndType(EntityType type, const Vec2& position, float radius);

	private:
		void findAndSolveEntityCollisions(Game *game, const World *world, Entity& entity);
		void solveEntityCollisionWithWorld(Game *game, const World *world, Entity& entity, Entity::Axis axis);

		void cleanUp(void);
		void removeEntity(int position);

		std::array<Entity, MAX_ENTITIES> entities;
		std::unordered_map<EntityId, Entity *> id_to_entity;
		std::array<EntityHandler, MAX_ENTITY_TYPES> type_to_handler;
		EntityFoundList found_entities;

		size_t num_entities;
		EntityId next_id;
};

#endif
