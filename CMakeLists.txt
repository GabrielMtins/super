cmake_minimum_required(VERSION 3.5)
project(super)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

file(GLOB_RECURSE all_SRCS
		"${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

add_executable(${PROJECT_NAME} ${all_SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall
    -Wextra
    -Wpedantic
)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

find_library(OPENGL OpenGL)
find_library(MATH_LIBRARY m)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${SDL2_MIXER_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/external/json/single_include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${MATH_LIBRARY}
)
