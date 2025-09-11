#ifndef ENGINEDEF_HPP
#define ENGINEDEF_HPP

#define ENTITY_MAX_TIMERS 4
#define ENTITY_MAX_FLAGS 4
#define ENTITY_MAX_CHILDREN 4

#define MAX_ENTITIES 1024
#define MAX_ENTITY_TYPES 64

#define MAX_INPUT 64
#define MAX_SPRITES 64

#define MAX_LAYERS 3
#define WORLD_LAYER_BG 0
#define WORLD_LAYER_FG 1
#define MAX_TILES (256 * 256)

using EntityId = int;
using EntityType = int;

#endif
