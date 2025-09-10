#!/bin/sh

if [ -z  "$1" ]; then
	echo "Error: No argument provided."
	exit 1
fi

file="./src/game/$1.cpp"
custom_entities_header="./include/game/CustomEntities.hpp"
custom_entities_modified="./src/game/CustomEntities.cpp"
default_file="./ExampleEntity.cpp"
allcaps=$(echo $1 | tr '[a-z]' '[A-Z]')

if [ -f "$file" ]; then
	echo "Error: file $file already exists."
	exit 1
fi

rm -f $custom_entities_modified

sed "s/ExampleEntity/$1/g; s/ENTITY_EXAMPLEENTITY/ENTITY_$allcaps/g" $default_file > $file

found_already=$(grep "$allcaps" $custom_entities_header)

if [ -z $found_already ]; then
	sed -i -e "/NUM_ENTITY_TYPE/i\ \tENTITY_$allcaps," -e "/#endif/i\\EntityHandler $1_GetHandler(void);\n" $custom_entities_header
fi

./generate_customentities.sh > $custom_entities_modified
