#include "core/Context.hpp"

Context::Context(void) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(
			480 * 3,
			270 * 3,
			SDL_WINDOW_SHOWN,
			&window,
			&renderer
			);

	SDL_RenderSetLogicalSize(renderer, 480, 270);

	running = true;
}

void Context::pollEvents(void) {
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
		}
	}
}

SDL_Renderer * Context::getRenderer(void) {
	return renderer;
}

bool Context::isRunning(void) {
	return running;
}

void Context::renderClear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);
}

void Context::renderPresent(void) {
	SDL_RenderPresent(renderer);
}

Context::~Context(void) {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
}
