#include "engine/EntityHandler.hpp"

EntityHandler::EntityHandler(void) {
	create = NULL;
	update = NULL;
	collision = NULL;
}

EntityHandler::EntityHandler(EntityCreateCallback create, EntityUpdateCallback update, EntityCollisionCallback collision) {
	this->create = create;
	this->update = update;
	this->collision = collision;
}
