#include "engine/EntityHandler.hpp"

EntityHandler::EntityHandler(void) {
	create = NULL;
	update = NULL;
}

EntityHandler::EntityHandler(EntityCreateCallback create, EntityUpdateCallback update) {
	this->create = create;
	this->update = update;
}
