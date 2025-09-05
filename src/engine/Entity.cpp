#include "engine/Entity.hpp"

Entity::Entity(void) {
	id = -1;
	alive = false;
	texture = NULL;
	texture_cell = 0;
	flip_x = false;
	flip_y = false;
	target = -1;
}

void Entity::render(Context *context) {
	if(texture == NULL)
		return;

	texture->renderCell(
			context,
			(int) (position.x - texture_offset.x),
			(int) (position.y - texture_offset.y),
			texture_cell,
			flip_x,
			flip_y
			);
}
