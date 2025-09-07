#include "core/Context.hpp"
#include "core/Console.hpp"

Context::Context(void) {
	window = NULL;
	renderer = NULL;
	running = false;
}

void Context::init(void) {
	int error = 0;

	internal_width = 480;
	internal_height = 270;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		console.error((std::string) "[Context] Failed to initialize SDL2: " + SDL_GetError());
		console.exit(1);
	}

	console.log("[Context] Initialized SDL2 successfully.");

	error = SDL_CreateWindowAndRenderer(
			internal_width * 3,
			internal_height * 3,
			SDL_WINDOW_SHOWN,
			&window,
			&renderer
			);

	if(error < 0) {
		console.error((std::string) "[Context] Failed to create window and renderer: " + SDL_GetError());
		console.exit(1);
	}

	console.log("[Context] Created window and renderer.");

	SDL_RenderSetLogicalSize(renderer, internal_width, internal_height);
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

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

void Context::quit(void) {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	console.log("[Context] Quitting SDL2.");
}

int Context::getInternalWidth(void) {
	return internal_width;
}

int Context::getInternalHeight(void) {
	return internal_height;
}
