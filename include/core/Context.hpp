#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>

class Context {
	public:
		Context(void);

		void pollEvents(void);
		SDL_Renderer * getRenderer(void);
		bool isRunning(void);
		void renderClear(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void renderPresent(void);

		~Context(void);

	private:
		SDL_Window *window;
		SDL_Renderer* renderer;

		SDL_Event event;
		bool running;
};

#endif
