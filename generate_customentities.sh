#!/bin/sh

custom_entities_hpp="./include/game/CustomEntities.hpp"

entity_list=$(grep "GetHandler" $custom_entities_hpp | sed "s/_GetHandler.*$//g; s/,//g" | awk '{print $2}')

echo "#include \"game/CustomEntities.hpp\""
echo

echo "void Custom_AddEntityTypes(Game *game) {"

for i in $entity_list; do
	all_caps=$(echo $i | tr "a-z" "A-Z")
	echo "\tgame->addHandlerToType(ENTITY_$all_caps, "$i"_GetHandler());"
done

echo "}"
