#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdio>

#include "core/CoreDef.hpp"

class Context {
	public:
		Context(void);

		void init(const std::string& title, int internal_width, int internal_height);
		void pollEvents(void);
		SDL_Renderer * getRenderer(void);
		bool isRunning(void) const;
		void renderClear(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void renderPresent(void);
		void quit(void);

		int getInternalWidth(void) const;
		int getInternalHeight(void) const;

		int getWindowWidth(void) const;
		int getWindowHeight(void) const;

		int getMouseX(void) const;
		int getMouseY(void) const;

	private:
		SDL_Window *window;
		SDL_Renderer* renderer;

		SDL_Event event;
		bool running;
		int internal_width, internal_height;
		int window_width, window_height;

		int mouse_x, mouse_y;
};

#endif
