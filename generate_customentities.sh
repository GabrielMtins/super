#!/bin/sh

custom_entities_hpp="./include/game/CustomEntities.hpp"

entity_list=$(grep "ENTITY_" $custom_entities_hpp | sed "s/ENTITY_//g; s/,//g" | head --lines=-1)

echo "#include \"game/CustomEntities.hpp\""
echo

echo "void Custom_AddEntityTypes(Game *game) {"

for i in $entity_list; do
	typed=$(echo $i | tr '[A-Z]' '[a-z]' | awk '{print toupper(substr($0, 1, 1)) substr($0, 2)}')
	echo "\tgame->addHandlerToType(ENTITY_$i, "$typed"_GetHandler());"
done

echo "}"
