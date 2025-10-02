#include "core/Context.hpp"
#include "core/Console.hpp"

Context::Context(void) {
	window = NULL;
	renderer = NULL;
	running = false;

	mouse_x = 0;
	mouse_y = 0;
}

void Context::init(const std::string& title, int internal_width, int internal_height) {
	this->internal_width = internal_width;
	this->internal_height = internal_height;

	this->window_width = internal_width * 6;
	this->window_height = internal_height * 6;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		console.error((std::string) "[Context] Failed to initialize SDL2: " + SDL_GetError());
		console.exit(1);
	}

	console.log("[Context] Initialized SDL2 successfully!");

	if(IMG_Init(IMG_INIT_PNG) < 0) {
		console.error((std::string) "[Context] Failed to initialize SDL2 image: " + SDL_GetError());
		console.exit(1);
	}

	console.log("[Context] Initialized SDL2 image successfully!");

	if(Mix_Init(MIX_INIT_MP3) < 0) {
		console.error((std::string) "[Context] Failed to initialize SDL2 mixer: " + SDL_GetError());
		console.exit(1);
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		console.error((std::string) "[Context] Failed to open audio: " + SDL_GetError());
		console.exit(1);
	}

	console.log("[Context] Initialized SDL2 mixer successfully!");

	if(TTF_Init() < 0) {
		console.error((std::string) "[Context] Failed to initialize SDL ttf: " + TTF_GetError());
		console.exit(1);
	}

	console.log("[Context] Initialized SDL2 ttf successfully!");

	window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			window_width,
			window_height,
			SDL_WINDOW_RESIZABLE
			);

	if(window == NULL) {
		console.error((std::string) "[Context] Failed to create window: " + SDL_GetError());
		console.exit(1);
	}

	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_PRESENTVSYNC
			);

	if(renderer == NULL) {
		console.error((std::string) "[Context] Failed to create renderer: " + SDL_GetError());
		console.exit(1);
	}

	console.log("[Context] Created window and renderer.");

	SDL_RenderSetLogicalSize(renderer, internal_width, internal_height);
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	//window_width = 1920;
	//window_height = 1080;
	SDL_ShowCursor(0);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	running = true;
}

void Context::pollEvents(void) {
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			case SDL_WINDOWEVENT_RESIZED:
				window_width = event.window.data1;
				window_height = event.window.data2;
				printf("\n\n?\n\n");
				break;

			case SDL_MOUSEMOTION:
				mouse_x = event.motion.x;
				mouse_y = event.motion.y;
				break;
		}
	}
}

SDL_Renderer * Context::getRenderer(void) {
	return renderer;
}

bool Context::isRunning(void) const {
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

	TTF_Quit();
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();

	console.log("[Context] Quitting SDL2.");
}

int Context::getInternalWidth(void) const {
	return internal_width;
}

int Context::getInternalHeight(void) const {
	return internal_height;
}

int Context::getWindowWidth(void) const {
	return window_width;
}

int Context::getWindowHeight(void) const {
	return window_height;
}

int Context::getMouseX(void) const {
	return mouse_x;
}
int Context::getMouseY(void) const {
	return mouse_y;
}
